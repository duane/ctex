#ifndef __INCLUDE_RENDER_RENDERNODE_H__
#define __INCLUDE_RENDER_RENDERNODE_H__

#include <cassert>
#include <stdint.h>

#include <Diag/Diag.h>
#include <Render/Char.h>
#include <Render/Box.h>
#include <Render/Glue.h>
#include <Render/Penalty.h>
#include <Type/Font.h>
#include <Util/UniquePtr.h>

namespace tex {

class State;

enum {
  NULL_NODE = 0,
  HBOX_NODE,
  VBOX_NODE,
  CHAR_NODE,
  GLUE_NODE,
  PENALTY_NODE,
};

class RenderNode {
// allow copy/assign.
public:
  RenderNode *link;
  uint32_t type;
  union {
    char_node ch;
    glue_node glue;
    box_node box;
    penalty_node penalty;
  };

  sp width(UniquePtr<State> &state) const;
  sp height(UniquePtr<State> &state) const;
  sp depth(UniquePtr<State> &state) const;
  sp shift(UniquePtr<State> &state) const;

  static RenderNode *char_rnode(uint8_t uc, uint8_t f) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = CHAR_NODE;
    node->ch = (char_node){uc, f};
    return node;
  }

  static RenderNode *glue_rnode(sp width, sp stretch, sp shrink,
                    glue_order stretch_order, glue_order shrink_order) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = GLUE_NODE;
    node->glue = (glue_node){width, stretch, shrink,
                             stretch_order, shrink_order};
    return node;
  }

  static RenderNode *new_glue(glue_node &g) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = GLUE_NODE;
    node->glue = g;
    return node;
  }

  static RenderNode *empty_hbox(void) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = empty_box();
    return node;
  }

  static RenderNode *hbox(box_node &box) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = box;
    return node;
  }

  static RenderNode *empty_vbox(void) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = HBOX_NODE;
    node->box = empty_box();
    return node;
  }

  static RenderNode *vbox(box_node &box) {
    RenderNode *node = new RenderNode;
    node->link = NULL;
    node->type = VBOX_NODE;
    node->box = box;
    return node;
  }
};

}  // namespace tex

#endif  // __INCLUDE_RENDER_RENDERNODE_H__