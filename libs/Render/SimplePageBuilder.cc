#include <Render/SimplePageBuilder.h>

#include <Render/Page.h>
#include <Render/RenderNode.h>
#include <State/State.h>

using namespace tex;

void SimplePageBuilder::ship_page(UniquePtr<State> &state) {
  Page *page = new Page();
  page->head = p_head;
  page->tail = p_tail;
  while (break_head) {
    RenderNode *next = break_head->link;
    delete break_head;
    break_head = next;
  }
  break_tail = NULL;
  cur_height = scaled(0);
  state->render().ship_page(page);
}

void SimplePageBuilder::build_page(UniquePtr <State> &state) {
  RenderState &render = state->render();
  assert(render.mode() == VMODE && "Called build_page in invalid mode.");

  sp target_height = state->mem(VSIZE_CODE).scaled;
  while (render.head()) {
    RenderNode *p = render.head();
    switch (p->type) {
      case GLUE_NODE: {
        break_height += p->glue.width;
        render.set_head(p->link);
        if (!break_head) {
          break_head = break_tail = p;
        } else {
          break_tail->link = p;
          break_tail = p;
          break_tail->link = NULL;
        }
        break;
      }
      case PENALTY_NODE: {
        if (p->penalty == PENALTY_BREAK)
          ship_page(state);
        render.set_head(p->link);
        delete p;
      }
      //case RULE_NODE:
      case VBOX_NODE:
      case HBOX_NODE: {
        sp box_height = p->height(state);
        if (cur_height + break_height + box_height >= target_height) {
          ship_page(state);
        }
        if (break_head) {
          if (p_head)
            p_tail->link = break_head;
          else
            p_head = p_tail = break_head;
          break_tail->link = p;
          render.set_head(p->link);
          p->link = NULL;
          break_head = break_tail = NULL;
          cur_height += break_height + box_height;
        } else {
          if (p_head)
            p_tail->link = p;
          else
            p_head = p_tail = p;
          render.set_head(p->link);
          cur_height += box_height;
        }
        break;
      }
      default: {
        assert(false && "Found bad node type when page breaking.");
      }
    }
  }
}