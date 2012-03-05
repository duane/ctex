#include <Render/RenderNode.h>
#include <State/State.h>

using namespace tex;

 sp RenderNode::width(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.width;
    case GLUE_NODE:
      return glue.width;
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).width;
    default:
      assert(false && "Attempted to fetch width of widthless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::height(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.height;
    case GLUE_NODE:
      return glue.width; // cheating.
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).height;
    default:
      assert(false && "Attempted to fetch width of heightless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::depth(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.depth;
    case CHAR_NODE:
      return state->metrics(ch.font).get(ch.ch).depth;
    default:
      assert(false && "Attempted to fetch width of depthless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}

sp RenderNode::shift(UniquePtr<State> &state) const {
  switch(type) {
    case HBOX_NODE:
    case VBOX_NODE:
      return box.width;
    default:
      assert(false && "Attempted to fetch width of shiftless node.");
  }
  assert(false && "Unreachable! Fix code.");
  return scaled(0);
}
