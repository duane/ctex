#ifndef __INCLUDE_RENDER_RENDERNODE_H__
#define __INCLUDE_RENDER_RENDERNODE_H__

#include <cassert>
#include <stdint.h>

#include <Diag/Diag.h>

namespace tex {

enum {
  NULL_NODE = 0,
  HLIST_NODE,
  VLIST_NODE,
  CHAR_NODE,
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
    char_node c;
  };
public:
  uint32_t type(void) const {
    return type_tag;
  }

  char_node c_node(void) const {
    assert(type_tag == CHAR_NODE && "Requested node data of invalid type.");
    return c;
  }

  RenderNode(void) : type_tag(NULL_NODE) {}

  RenderNode(const RenderNode &other) {
    link = NULL;
    type_tag = other.type();
    switch(type_tag) {
      case CHAR_NODE: {
        c = other.c_node();
        break;
      }
      default:
        throw new GenericDiag("This node type is not handled yet.",
                              DIAG_RENDER_ERR, BLAME_HERE);
    }
  }

  static RenderNode char_rnode(uint8_t uc, uint8_t f) {
    RenderNode node;
    node.link = NULL;
    node.type_tag = CHAR_NODE;
    node.c = (char_node){uc, f};
    return node;
  }
};

}  // namespace tex

#endif  // __INCLUDE_RENDER_RENDERNODE_H__