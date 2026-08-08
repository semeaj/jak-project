#include "OceanMidAndFar.h"

#include "common/dma/gs.h"

#include "game/graphics/opengl_renderer/ocean/CommonOceanRenderer.h"

#include "third-party/imgui/imgui.h"

OceanMidAndFar::OceanMidAndFar(const std::string& name, int my_id)
    : BucketRenderer(name, my_id),
      m_direct(name, my_id, 4096),
      m_envmap_renderer(name + "-envmap", my_id, 4096),
      m_texture_renderer(true) {}

void OceanMidAndFar::draw_debug_window() {
  if (ImGui::TreeNode("envmap")) {
    m_envmap_renderer.draw_debug_window();
    ImGui::TreePop();
  }
  m_texture_renderer.draw_debug_window();
  m_direct.draw_debug_window();
}

void OceanMidAndFar::init_textures(TexturePool& pool, GameVersion version) {
  m_texture_renderer.init_textures(pool, version);
  m_envmap_renderer.init_textures(pool, version);
}

void OceanMidAndFar::render(DmaFollower& dma,
                            SharedRenderState* render_state,
                            ScopedProfilerNode& prof) {
  // skip if disabled
  if (!m_enabled) {
    while (dma.current_tag_offset() != render_state->next_bucket) {
      dma.read_and_advance();
    }
    return;
  }

  switch (render_state->version) {
    case GameVersion::Jak1:
      render_jak1(dma, render_state, prof);
      break;
    case GameVersion::Jak2:
    case GameVersion::Jak3:
      render_jak2(dma, render_state, prof);
      break;
    case GameVersion::JakX:
      render_jakx(dma, render_state, prof);
      break;
  }
}

void OceanMidAndFar::render_jakx(DmaFollower& dma,
                                 SharedRenderState* render_state,
                                 ScopedProfilerNode& prof) {
  // jakx bucket 6 (emitters in goal_src/jakx/engine/gfx/ocean/):
  //  - update-map's 4-qw direct: FRAME_1/ZBUF_1/SCISSOR_1 A+D setup for the viewport
  //  - draw-ocean-far, only when far-on: an 11-qw gs-set (test/alpha/tex0/tex1/miptbp1/
  //    miptbp2/texa/clamp/fogcol/texflush, texturing from sky-texture-anim slot 3), then
  //    one variable-size direct holding the render-ocean-quad GIF packets (just the 1-qw
  //    close-sky-buffer terminator when every far quad is culled)
  //  - draw-ocean-mid: base/offset, constants, VU-call grammar identical to jak2's
  //    (same 0x2dd constants address, same 0/46/73/107/275/41/43 entry points)
  //  - end-buffer!: a 2-qw texa restore, which is also the mid parser's end tag
  // Unlike jak2/jak3 there is no envmap render or ocean-texture section: jakx never
  // shipped ocean-texture.o (the ocean texture routes through the texture animator) and
  // its envmap is an ordinary pool texture referenced from the mid/near constants. The
  // VU program upload (dma-buffer-add-vu-function) emits nothing on PC.
  auto data0 = dma.read_and_advance();
  ASSERT(data0.vif1() == 0 || data0.vifcode1().kind == VifCode::Kind::NOP);
  ASSERT(data0.vif0() == 0 || data0.vifcode0().kind == VifCode::Kind::MARK);
  ASSERT(data0.size_bytes == 0);

  if (dma.current_tag_offset() == render_state->next_bucket) {
    return;
  }
  m_direct.reset_state();

  handle_ocean_far_jakx(dma, render_state, prof);
  m_direct.flush_pending(render_state, prof);

  m_direct.set_mipmap(true);
  handle_ocean_mid_jakx(dma, render_state, prof);

  // Bucket epilogue: every opened jakx bucket closes with the end tag, then a dma CALL
  // into the shared default-regs restore (one 12-qw FLUSHA/DIRECT with 11 A+D registers)
  // and its RET. GS state does not carry across buckets on PC, so the restore is
  // consumed, not rendered; jak2 skips its (shorter) equivalent the same way.
  consume_bucket_epilogue_jakx(dma, render_state);

  m_direct.flush_pending(render_state, prof);
  m_direct.set_mipmap(false);
}

void OceanMidAndFar::handle_ocean_far_jakx(DmaFollower& dma,
                                           SharedRenderState* render_state,
                                           ScopedProfilerNode& prof) {
  // update-map's viewport GS context: one 4-qw direct with FRAME_1/ZBUF_1/SCISSOR_1.
  auto hdr = dma.read_and_advance();
  ASSERT(hdr.size_bytes == 64);
  ASSERT(hdr.vifcode0().kind == VifCode::Kind::NOP);
  ASSERT(hdr.vifcode1().kind == VifCode::Kind::DIRECT);
  ASSERT(hdr.vifcode1().immediate == 4);
  m_direct.render_gif(hdr.data, hdr.size_bytes, render_state, prof);

  // draw-ocean-far's transfers (absent when far-on is #f; the gs-set alone is also
  // skipped when the sky-texture-anim slot-3 texture is #f). Everything up to the mid
  // base/offset tag is direct GIF data.
  while (dma.current_tag().kind == DmaTag::Kind::CNT &&
         dma.current_tag_vifcode0().kind == VifCode::Kind::NOP) {
    auto data = dma.read_and_advance();
    ASSERT(data.vifcode1().kind == VifCode::Kind::DIRECT);
    ASSERT(data.size_bytes / 16 == data.vifcode1().immediate);

    // draw-ocean-far's 11-qw gs-set carries texa with ta0 = 0x80; patch ta0 to 0 the
    // same way handle_ocean_far does for jak1/jak2's 160-byte init (see the note
    // there). Identify it by its A+D giftag; the far quad data uses 3-reg packed tags.
    if (data.size_bytes == 176 && GifTag(data.data).nreg() == 1 &&
        data.data[112 + 8] == (u8)GsRegisterAddress::TEXA) {
      u8 patched[176];
      memcpy(patched, data.data, 176);
      patched[112] = 0;  // texa ta0
      m_direct.render_gif(patched, 176, render_state, prof);
    } else {
      m_direct.render_gif(data.data, data.size_bytes, render_state, prof);
    }
  }
}

void OceanMidAndFar::handle_ocean_mid_jakx(DmaFollower& dma,
                                           SharedRenderState* render_state,
                                           ScopedProfilerNode& prof) {
  // draw-ocean-mid always runs when the bucket is non-empty, and its VU upload is empty
  // on PC, so the base/offset tag must be next. The mid grammar itself is jak2's.
  ASSERT(dma.current_tag_vifcode0().kind == VifCode::Kind::BASE);
  m_mid_renderer.run_jak2(dma, render_state, prof);

  // run_jak2 stops at end-buffer!'s 2-qw texa restore; consume it. jak2 skips this
  // transfer without rendering it too (GS state does not carry across PC buckets).
  auto texa = dma.read_and_advance();
  ASSERT(texa.size_bytes == 32);
  ASSERT(texa.vifcode0().kind == VifCode::Kind::NOP);
  ASSERT(texa.vifcode1().kind == VifCode::Kind::DIRECT);
}

void OceanMidAndFar::render_jak1(DmaFollower& dma,
                                 SharedRenderState* render_state,
                                 ScopedProfilerNode& prof) {
  // jump to bucket
  auto data0 = dma.read_and_advance();
  ASSERT(data0.vif1() == 0);
  ASSERT(data0.vif0() == 0);
  ASSERT(data0.size_bytes == 0);

  // see if bucket is empty or not
  if (dma.current_tag().kind == DmaTag::Kind::CALL) {
    // renderer didn't run, let's just get out of here.
    for (int i = 0; i < 4; i++) {
      dma.read_and_advance();
    }
    ASSERT(dma.current_tag_offset() == render_state->next_bucket);
    return;
  }
  m_direct.reset_state();

  {
    auto p = prof.make_scoped_child("texture");
    m_texture_renderer.handle_ocean_texture_jak1(dma, render_state, p);
  }

  handle_ocean_far(dma, render_state, prof);
  m_direct.flush_pending(render_state, prof);

  m_direct.set_mipmap(true);
  handle_ocean_mid(dma, render_state, prof);

  auto final_next = dma.read_and_advance();
  ASSERT(final_next.vifcode0().kind == VifCode::Kind::NOP &&
         final_next.vifcode1().kind == VifCode::Kind::NOP && final_next.size_bytes == 0);
  for (int i = 0; i < 4; i++) {
    dma.read_and_advance();
  }
  ASSERT(dma.current_tag_offset() == render_state->next_bucket);

  m_direct.flush_pending(render_state, prof);
  m_direct.set_mipmap(false);
}

void OceanMidAndFar::render_jak2(DmaFollower& dma,
                                 SharedRenderState* render_state,
                                 ScopedProfilerNode& prof) {
  // jump to bucket
  auto data0 = dma.read_and_advance();
  ASSERT(data0.vif1() == 0 || data0.vifcode1().kind == VifCode::Kind::NOP);
  ASSERT(data0.vif0() == 0 || data0.vifcode0().kind == VifCode::Kind::MARK);
  ASSERT(data0.size_bytes == 0);

  // see if bucket is empty or not
  if (dma.current_tag_offset() == render_state->next_bucket) {
    // fmt::print("ocean-mid-far: early exit!\n");
    return;
  }
  m_direct.reset_state();

  // the progress menu draws directly renderable packets into this bucket from process context,
  // ahead of any ocean chain the draw hook appends, and during menus the ocean skips emission
  // entirely. every real ocean chain starts with a set-display-gs-state packet writing SCISSOR_1
  // (the 64x64 envmap or 128x128 texture setup), so render everything before the first SCISSOR_1
  // write through the direct renderer.
  {
    auto child = prof.make_scoped_child("direct-head");
    bool got_direct = false;
    while (dma.current_tag_offset() != render_state->next_bucket) {
      DmaFollower peek = dma;
      auto data = peek.read_and_advance();
      u64 scissor;
      if (scan_gs_set(data.data, data.size_bytes, GsRegisterAddress::SCISSOR_1, &scissor)) {
        // the ocean chain begins here, leave it for the ocean renderers.
        break;
      }
      dma = peek;
      got_direct = true;
      m_direct.render_vif(data.vif0(), data.vif1(), data.data, data.size_bytes, render_state,
                          child);
    }
    if (got_direct) {
      m_direct.flush_pending(render_state, child);
    }
  }

  // menus suppress ocean emission, so the bucket may have contained only menu draws.
  if (dma.current_tag_offset() == render_state->next_bucket) {
    return;
  }

  {
    auto p = prof.make_scoped_child("envmap");
    m_envmap_renderer.handle_ocean_envmap_jak2(dma, render_state, p);
  }

  {
    auto p = prof.make_scoped_child("texture");
    m_texture_renderer.handle_ocean_texture_jak2(dma, render_state, p);
  }

  handle_ocean_far(dma, render_state, prof);
  m_direct.flush_pending(render_state, prof);

  m_direct.set_mipmap(true);
  handle_ocean_mid(dma, render_state, prof);

  auto final_next = dma.read_and_advance();
  ASSERT(final_next.vifcode0().kind == VifCode::Kind::NOP &&
         final_next.vifcode1().kind == VifCode::Kind::NOP && final_next.size_bytes == 0);
  for (int i = 0; i < 2; i++) {
    dma.read_and_advance();
  }
  ASSERT(dma.current_tag_offset() == render_state->next_bucket);

  // auto transfers = 0;
  // // print the entire chain
  // printf("START OCEAN MID FAR DMA!!!!!!!\n");
  // while (dma.current_tag_offset() != render_state->next_bucket) {
  //   auto data = dma.read_and_advance();
  //   printf(
  //       "dma transfer %d:\n%ssize: %d\nvif0: %s, data: %d\nvif1: %s, data: %d, imm: "
  //       "%d\n\n",
  //       transfers, dma.current_tag().print().c_str(), data.size_bytes,
  //       data.vifcode0().print().c_str(), data.vif0(), data.vifcode1().print().c_str(),
  //       data.vifcode1().num, data.vifcode1().immediate);
  //   transfers++;
  // }
  // printf("transfers: %d\n\n", transfers);

  m_direct.flush_pending(render_state, prof);
  m_direct.set_mipmap(false);
}

void OceanMidAndFar::handle_ocean_far(DmaFollower& dma,
                                      SharedRenderState* render_state,
                                      ScopedProfilerNode& prof) {
  auto init_data = dma.read_and_advance();
  ASSERT(init_data.size_bytes == 160);
  u8 init_data_buffer[160];
  memcpy(init_data_buffer, init_data.data, 160);

  // this is a bit of a hack, but it patches the ta0 to 0 in
  // (set! (-> (the-as (pointer gs-texa) s4-0) 8) (new 'static 'gs-texa :ta0 #x80 :ta1 #x80))
  // TODO figure out if we actually have do something here.
  u8 val = 0;
  memcpy(init_data_buffer + 80, &val, 1);
  m_direct.render_gif(init_data_buffer, 160, render_state, prof);

  while (dma.current_tag().kind == DmaTag::Kind::CNT &&
         dma.current_tag_vifcode0().kind == VifCode::Kind::NOP) {
    auto data = dma.read_and_advance();
    ASSERT(data.vifcode0().kind == VifCode::Kind::NOP);
    ASSERT(data.vifcode1().kind == VifCode::Kind::DIRECT);
    ASSERT(data.size_bytes / 16 == data.vifcode1().immediate);
    m_direct.render_gif(data.data, data.size_bytes, render_state, prof);
  }
}

bool is_end_tag(const DmaTag& tag, const VifCode& v0, const VifCode& v1) {
  return tag.qwc == 0 && tag.kind == DmaTag::Kind::NEXT && v0.kind == VifCode::Kind::NOP &&
         v1.kind == VifCode::Kind::NOP;
}
void OceanMidAndFar::handle_ocean_mid(DmaFollower& dma,
                                      SharedRenderState* render_state,
                                      ScopedProfilerNode& prof) {
  if (dma.current_tag_vifcode0().kind == VifCode::Kind::BASE) {
    switch (render_state->version) {
      case GameVersion::Jak1:
        m_mid_renderer.run(dma, render_state, prof);
        break;
      case GameVersion::Jak2:
      case GameVersion::Jak3:
      case GameVersion::JakX:
        m_mid_renderer.run_jak2(dma, render_state, prof);
    }
  } else {
    // not drawing
    return;
  }

  while (!is_end_tag(dma.current_tag(), dma.current_tag_vifcode0(), dma.current_tag_vifcode1())) {
    dma.read_and_advance();
  }
}