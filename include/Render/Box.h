#ifndef __INCLUDE_RENDER_BOX_H__
#define __INCLUDE_RENDER_BOX_H__

#include <Render/Glue.h>

namespace tex {

class RenderNode;

struct box_node {
  sp width, depth, height, shift;
  RenderNode *list;
  glue_order order;
  glue_sign sign;
};

static inline box_node empty_box(void) {
  return (box_node){scaled(0), scaled(0), scaled(0), scaled(0),
                    NULL, GLUE_NORMAL, SIGN_NORMAL};
}

}  // namespace tex

#endif  //  __INCLUDE_RENDER_BOX_H__