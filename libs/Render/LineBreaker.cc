
#include <Render/LineBreaker.h>
#include <Render/RenderNode.h>

using namespace tex;

/*
 * This algorithm taken from Part 38-39 of the TeXBook,
 * "Breaking paragraphs into lines." This code is a straightforward port.
 */

enum fitness {
  VERY_LOOSE_FIT = 0,
  LOOSE_FIT,
  DECENT_FIT,
  TIGHT_FIT,
};

enum active_type {
  UNHYPH = 0,
  HYPH,
  DELTA
};

const uint32_t AWFUL_BAD = 0x3fffffff;

struct passive_node {
  passive_node *link;
  RenderNode *cur_break;
  passive_node *prev_break;
};

struct active_break {
  passive_node *break_node;
  uint32_t line_number;
  fitness fit;
  int32_t total_demerits;
};


enum {
  DELTA_NORMAL = 0,
  DELTA_PT,
  DELTA_FIL,
  DELTA_FILL,
  DELTA_FILLL,
  DELTA_SHRINK
}

struct delta {
  sp width[6];

  void operator+=(const delta &other) {
    for (unsigned i = 0; i < 6; i++) {
      width[i] = width[i] + other.width[i];
    }
  }

  delta operator+(const delta &other) {
    delta result;
    for (unsigned i = 0; i < 6; i++){
      result.width[i] = width[i] + other.width[i];
    }
    return result;
  }

  delta operator-(const delta &other) {
    delta result;
    for (unsigned i = 0; i < 6; i++) {
      result.width[i] = width[i] - other.width[i];
    }
    return result;
  }
};

struct active_node {
  active_node *link;
  active_type type;
  union {
    active_break a;
    delta del;
  };
};

class LineBreaker {
public:
  active_node *active;
  active_node *last_active;
  passive_node *passive;
  RenderNode *cur_p, *prev_p;

  delta active_width; 
  delta cur_active_width;
  delta background;
  delta break_width;

  uint32_t minimum_demerits;
  uint32_t minimal_demerits[4];

  uint32_t threshold; // maximum badness.

  uint32_t easy_line;
  uint32_t old_l; // "old" line-number.

  LineBreaker(RenderNode *hlist) :
    active(NULL), last_active(NULL), passive(NULL), cur_p(hlist) {
    for (unsigned i = 0; i < 6; i++) {
      cur_active_width[i] = scaled(0);
    }

    // demerits
    minimum_demerits = AWFUL_BAD;
    for (unsigned i = 0; i < sizeof(minimal_demerits); i++) {
      minimal_demerits[i] = AWFUL_BAD;
    }
  }

  void try_break(active_type break_type) {
    active_node *cur_a, *prev_a, *prev_prev_a;
    RenderNode *s;

    bool no_break_yet = true;
    while (true) {
      cur_a = prev_a->link;
      if (cur_a->active_type == DELTA) {
        cur_active_width += cur_a->del;
        prev_prev_a = prev_a;
        prev_a = prev_prev_a->link;
        continue;
      }

      uint32_t l = cur_a->line_number;
      if (l > old_l) {
        if (minimum_demerits < AWFUL_BAD &&
            (old_l != easy_line || cur_a == last_active)) {
          // Create new active nodes for the best feasible breaks just found.
          if (no_break_yet) {
            // Compute the values of break_width.
            no_break_yet = false;
            break_width = background;
            s = cur_p;
            while (s) {
              bool done = false;
              if (s->type == GLUE_NODE) {
                  // Subtract glue from break_width
                  glue_node &glue = s->glue;
                  break_width.width[DELTA_NORMAL] -= glue.width;
                  break_width.width[DELTA_PT + glue.stretch_order] -= glue.stretch;
                  break_width.width[DELTA_SHRINK] -= glue.shrink;
            } else if (s->type == PENALTY_NODE)
              continue;
            else
              break;
          }

          // insert a delta node to prepare for breaks at cur_p.
          

          if (r == last_active)
            return;
        }
      }
    }
  }

  void break(UniquePtr<State> &state) {
    RenderNode *temp_head = state->render().head();
    if (!temp_head)
      throw new GenericDiag("Attempted to break NULL list.",
                            DIAG_RENDER_ERR, BLAME_HERE);


    // Find optimal breakpoints.
    uint32_t threshold = state->mem(PRETOLERANCE_CODE);
    // only single pass for now.
    // while (true) {
    cur_p = temp_head;
    bool auto_breaking = true;
    prev_p = cur_p;
    while (cur_p && active->link != NULL)

    //} // while (true)

  }
};

void line_break(UniquePtr<State> &state) {
  LineBreaker breaker(state->render().head());
  breaker.break(state);
}