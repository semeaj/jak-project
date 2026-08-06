#include "Generic2BucketRenderer.h"

#include "common/log/log.h"

Generic2BucketRenderer::Generic2BucketRenderer(const std::string& name,
                                               int id,
                                               std::shared_ptr<Generic2> renderer,
                                               Generic2::Mode mode)
    : BucketRenderer(name, id), m_generic(renderer), m_mode(mode) {}

void Generic2BucketRenderer::draw_debug_window() {
  m_generic->draw_debug_window();
}

void Generic2BucketRenderer::render(DmaFollower& dma,
                                    SharedRenderState* render_state,
                                    ScopedProfilerNode& prof) {
  // if the user has asked to disable the renderer, just advance the dma follower to the next
  // bucket and return immediately.
  if (!m_enabled) {
    while (dma.current_tag_offset() != render_state->next_bucket) {
      dma.read_and_advance();
    }
    return;
  }

  // jakx bring-up guard (#57 rung 4): the generic DMA parsers are wall-to-wall asserts
  // and the jakx bucket shape is unproven until the mercneric guards open at rung 5,
  // so even the empty-bucket walk could kill the boot across the 213 newly registered
  // destinations. Peek a copy of the follower first: a bucket that is empty in the
  // known shape (NOP tags, then the engine's CALL plus 4 reset tags into the next
  // bucket) is skipped without touching Generic2, a bucket carrying real data goes to
  // the parser loudly as intended, and an unrecognized empty shape logs once and skips
  // instead of dying. Retire once #57's acceptance captures prove the jakx shape.
  if (render_state->version == GameVersion::JakX) {
    DmaFollower peek = dma;
    bool has_data = false;
    bool known_shape = true;
    while (peek.current_tag_offset() != render_state->next_bucket) {
      auto tag = peek.current_tag();
      if (tag.qwc == 0 && peek.current_tag_vifcode0().kind == VifCode::Kind::NOP &&
          peek.current_tag_vifcode1().kind == VifCode::Kind::NOP) {
        if (tag.kind == DmaTag::Kind::CALL) {
          for (int i = 0; i < 4 && peek.current_tag_offset() != render_state->next_bucket; i++) {
            peek.read_and_advance();
          }
          known_shape = peek.current_tag_offset() == render_state->next_bucket;
          break;
        }
        peek.read_and_advance();
      } else {
        has_data = true;
        break;
      }
    }
    if (!has_data) {
      if (!known_shape && !m_jakx_shape_warned) {
        m_jakx_shape_warned = true;
        lg::warn("Generic2BucketRenderer {}: unrecognized empty-bucket DMA shape, skipping",
                 name_and_id());
      }
      while (dma.current_tag_offset() != render_state->next_bucket) {
        dma.read_and_advance();
      }
      m_empty = true;
      return;
    }
    if (!m_jakx_data_seen) {
      m_jakx_data_seen = true;
      lg::info("Generic2BucketRenderer {}: first real generic data", name_and_id());
    }
  }

  m_generic->render_in_mode(dma, render_state, prof, m_mode);
  m_empty = m_generic->empty();
}

bool Generic2BucketRenderer::empty() const {
  return m_empty;
}
