#ifndef __INCLUDE_RENDER_RENDERNODE_H__
#define __INCLUDE_RENDER_RENDERNODE_H__

#include <cassert>
#include <stdint.h>

#include <Diag/Diag.h>
#include <Render/GLue.h>

namespace tex {

enum {
  NULL_NODE = 0,
  HLIST_NODE,
  VLIST_NODE,
  CHAR_NODE,
  GLUE_NODE,
};

struct char_node {
  uint8_t c;
  uint8_t f;
};

class RenderNode {
// allow copy/assign.
public:
  RenderNode *link;
  uint32_t type_tag;
  union {
    char_node ch;
    glue_node glue;
  };
public:
  uint32_t type(void) const {
    return type_tag;
  }

  RenderNode(void) : link(NULL), type_tag(NULL_NODE) {}

  static RenderNode char_rnode(uint8_t uc, uint8_t f) {
    RenderNode node;
    node.link = NULL;
    node.type_tag = CHAR_NODE;
    node.ch = (char_node){uc, f};
    return node;
  }

  static RenderNode glue_rnode(sp width, sp stretch, sp shrink,
                    glue_order stretch_order, glue_order shrink_order) {
    RenderNode node;
    node.type_tag = GLUE_NODE;
    node.glue = (glue_node){width, stretch, shrink, stretch_order, shrink_order};
    return node;
  }
};

}  // namespace tex

#endif  // __INCLUDE_RENDER_RENDERNODE_H__