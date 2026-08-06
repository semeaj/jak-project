#pragma once

#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/opengl_renderer/foreground/Generic2.h"

class Generic2BucketRenderer : public BucketRenderer {
 public:
  Generic2BucketRenderer(const std::string& name,
                         int id,
                         std::shared_ptr<Generic2> renderer,
                         Generic2::Mode mode);
  void render(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof) override;
  void draw_debug_window() override;
  bool empty() const override;

 private:
  std::shared_ptr<Generic2> m_generic;
  Generic2::Mode m_mode;
  bool m_empty = false;
  // jakx bring-up (#57 rung 4): one warning per bucket when the empty-bucket walk
  // sees a shape the parser would have died on. Retires with the wrapper in render().
  bool m_jakx_shape_warned = false;
  // jakx bring-up (#57 rung 5): one line on the first frame this destination carries
  // real generic data, so a naturally firing effect identifies itself in the boot log.
  // Retires with the wrapper.
  bool m_jakx_data_seen = false;
};
