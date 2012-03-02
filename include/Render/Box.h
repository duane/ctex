#ifndef __INCLUDE_RENDER_BOX_H__
#define __INCLUDE_RENDER_BOX_H__

#include <Render/Glue.h>
#include <Util/UniquePtr.h>

namespace tex {

class RenderNode;
class State;

struct box_node {
  sp width, depth, height, shift;
  RenderNode *list;
  glue_order order;
  glue_sign sign;
  glue_ratio g_ratio;
};

static inline box_node empty_box(void) {
  return (box_node){scaled(0), scaled(0), scaled(0), scaled(0),
                    NULL, GLUE_NORMAL, SIGN_NORMAL, 0.0};
}

enum pack_type {
  EXACTLY,
  ADDITIONAL
};

extern RenderNode *hpack(UniquePtr<State> &state, RenderNode *hlist, sp width, pack_type type);

}  // namespace tex

#endif  //  __INCLUDE_RENDER_BOX_H__