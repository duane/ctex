#include <Render/Box.h>
#include <Render/RenderNode.h>
#include <State/State.h>

using namespace tex;

RenderNode *tex::hpack(UniquePtr<State> &state,
                  RenderNode *hlist, sp width, pack_type type) {
  RenderNode *result_node = RenderNode::empty_hbox();
  box_node &hbox = result_node->box;
  hbox.list = hlist;
  RenderNode *p;
  p = hlist;
  sp total_stretch[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};
  sp total_shrink[4] = {scaled(0), scaled(0), scaled(0), scaled(0)};
  while (p) {
    if (p->type == CHAR_NODE) {
      while (p && p->type == CHAR_NODE) {
        char_node &ch = p->ch;
        const CharInfo &info = state->metrics(ch.font).get(ch.ch);
        hbox.width += info.width;
        if (info.height > hbox.height)
          hbox.height = info.height;
        if (info.depth > hbox.depth)
          hbox.depth = info.depth;
        p = p->link;
      }
    }
    if (!p)
      break;
    switch (p->type) {
      // case RULE_NODE:
      case HBOX_NODE:
      case VBOX_NODE: {
        hbox.width += p->width(state);
        sp s = p->shift(state);
        sp h = p->height(state);
        sp d = p->depth(state);
        if ((h - s) > hbox.height)
          hbox.height = h - s;
        if ((d + s) > hbox.depth)
          hbox.depth = h + s;
        break;
      }
      case GLUE_NODE: {
        hbox.width += p->width(state);
        glue_node &glue = p->glue;
        total_stretch[glue.stretch_order] += glue.stretch;
        total_shrink[glue.shrink_order] += glue.shrink;
        break;
      }
      default:
        assert(false && "Attempted to pack unknown RenderNode type.");
    }
    p = p->link;
  }

  // hbox.width is now the "natural" width of the box
  sp target_width; // the width the hbox will be in the end
  if (type == EXACTLY)
    target_width = width;
  else
    target_width = hbox.width + width;
  sp delta_width = target_width - hbox.width;
  if (delta_width == 0) {
    hbox.order = GLUE_NORMAL;
    hbox.sign = SIGN_NORMAL;
    hbox.g_ratio = 0.0;
    return result_node;
  }
  if (delta_width < 0) { // shrink
    glue_order ord;
    if (total_shrink[GLUE_FILLL] != 0)
      ord = GLUE_FILLL;
    else if (total_shrink[GLUE_FILL] != 0)
      ord = GLUE_FILL;
    else if (total_shrink[GLUE_FIL] != 0)
      ord = GLUE_FIL;
    else
      ord = GLUE_NORMAL;

    if (total_shrink[ord] == 0) {
      hbox.order = GLUE_NORMAL;
      hbox.sign = SIGN_NORMAL;
      hbox.g_ratio = 0.0;
      return result_node;
    } else {
      hbox.order = ord;
      hbox.sign = SIGN_SHRINK;
      hbox.g_ratio = (float)(-delta_width.i64)/total_shrink[ord].i64;
      return result_node;
    }
  } else { // stretch
    glue_order ord;
    if (total_stretch[GLUE_FILLL] != 0)
      ord = GLUE_FILLL;
    else if (total_stretch[GLUE_FILL] != 0)
      ord = GLUE_FILL;
    else if (total_stretch[GLUE_FIL] != 0)
      ord = GLUE_FIL;
    else
      ord = GLUE_NORMAL;

    if (total_stretch[ord] == 0) {
      hbox.order = GLUE_NORMAL;
      hbox.sign = SIGN_NORMAL;
      hbox.g_ratio = 0.0;
      return result_node;
    } else {
      hbox.order = ord;
      hbox.sign = SIGN_STRETCH;
      hbox.g_ratio = (float)delta_width.i64/total_shrink[ord].i64 ;
      return result_node;
    }
    assert(false && "Unreachable! please fix.");
    return NULL;
  }



}