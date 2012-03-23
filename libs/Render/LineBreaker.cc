/*****************************************************************************
*  Copyright (c) 2012 Duane Ryan Bailey                                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*****************************************************************************/

#include <Render/LineBreaker.h>
#include <Render/RenderNode.h>
#include <Render/SimpleBreaker.h>

#include <cstdlib>
#include <iostream>

namespace tex {

typedef scaled_vector<6> delta;

enum active_type {
  ACTIVE,
  DELTA,
};

enum fitness_type {
  VERY_LOOSE_FIT,
  LOOSE_FIT,
  DECENT_FIT,
  TIGHT_FIT,
  MAX_FITNESS,
};

struct passive_node {
  RenderNode *break_point;
  passive_node *parent, *link;
};

struct active_node {
  active_type type;
  active_node *link, *prev;
  union {
    struct {
      passive_node *passive;
      fitness_type fitness;
      int32_t demerits;
      int32_t line_number;
    };
    delta d;
  };

  void unlink(void) {
    if (prev)
      prev->link = link;
    if (link)
      link->prev = prev;
  }

  void insert_before(active_node *node) {
    node->prev = prev;
    node->link = this;
    if (prev)
      prev->link = node;
    prev = node;
  }

  void insert_after(active_node *node) {
    node->link = link;
    node->prev = this;
    if (link)
      link->prev = node;
    link = node;
  }
};

class active_list {
public:
  active_node *head, *tail;

  active_list(void) : head(NULL), tail(NULL) {}

  void append(active_node *node) {
    node->link = NULL;
    node->prev = NULL;
    if (head) {
      tail->link = node;
      node->prev = tail;
    } else
      head = tail = node;
  }
};

enum {
  DELTA_WIDTH = 0,
  DELTA_NORMAL,
  DELTA_FIL,
  DELTA_FILL,
  DELTA_FILLL,
  DELTA_SHRINK,
};

class LineBreaker {
private:
  delta active_width, cur_active_width, background, break_width;
  RenderNode *head, *tail, *cur_p, *prev_p;

  active_list active;
  active_node *best_bet;
  passive_node *passive;

  int32_t threshold;
  int32_t line_penalty;
  int32_t minimum_demerits;
  int32_t minimal_demerits[MAX_FITNESS];
  passive_node *best_place[MAX_FITNESS];
  int32_t best_place_line[MAX_FITNESS];

  fitness_type fit_class;

  sp line_width, left_skip, right_skip;

  bool no_break_yet;

  bool final_pass;

public:
  void print_active_nodes(UniquePtr<State> &state) {
    active_node *act = active.head;
    std::cout << "Active nodes:\n";
    while (act) {
      if (act->type == DELTA)
        std::cout << "\tDelta: " << act->d[0].string() << "\n";
      else
        std:: cout << "\tActive: " << act->demerits << "\n";
      act = act->link;
    }
  }

  void add_nodes_if_necessary(UniquePtr<State> &state, active_node *&r) {
    if (minimum_demerits < PENALTY_AWFUL) {
      // TeX@836
      if (no_break_yet) {
        // TeX@837
        no_break_yet = false;
        break_width = background;
        RenderNode *s = cur_p;
        while (s && s->type == GLUE_NODE) {
          break_width[0] -= s->glue.width;
          break_width[s->glue.stretch_order + 1] -= s->glue.stretch;
          break_width[5] -= s->glue.shrink;
          s = s->link;
        }
        // end TeX@837
      }
      // TeX@843
      active_node *prev_r;
      if (!active.head)
        prev_r = NULL;
      else if (!r)
        prev_r = active.tail;
      else
        prev_r = r->prev;

      if (!prev_r) {
        active_width = break_width;
      } else {
        if (prev_r->type == DELTA) {
          prev_r->d -= (cur_active_width + break_width);
        } else {
          active_node *new_delta = new active_node;
          new_delta->d = break_width - cur_active_width;
          new_delta->type = DELTA;
          prev_r->insert_after(new_delta);
          if (active.tail->link)
            active.tail = active.tail->link;
          prev_r = prev_r->link;
        }
      }

      // end TeX@843;

      int32_t abs_adj_demerits = abs(state->eqtb()[ADJ_DEMERITS_CODE].i64);
      if (abs_adj_demerits >= (PENALTY_AWFUL - minimum_demerits))
        minimum_demerits = PENALTY_AWFUL - 1;
      else
        minimum_demerits += abs_adj_demerits;
      for (unsigned fit = 0; fit < MAX_FITNESS; fit++) {
        if (minimal_demerits[fit] < minimum_demerits) {
          std::cout << "Creating new node.\n";
          // TeX@845
          passive_node *new_passive = new passive_node;
          new_passive->break_point = cur_p;
          new_passive->parent = best_place[fit];
          new_passive->link = passive;
          passive = new_passive;

          active_node *new_active = new active_node;
          new_active->type = ACTIVE;
          new_active->passive = passive;
          new_active->line_number = best_place_line[fit] + 1;
          new_active->fitness = (fitness_type)fit;
          new_active->demerits = minimal_demerits[fit];
          if (!prev_r) {
            active.append(new_active);
          } else {
            prev_r->insert_after(new_active);
            if (!new_active->link)
              active.tail = new_active;
            prev_r = prev_r->link;
          }
          // end TeX@845
          minimal_demerits[fit] = PENALTY_AWFUL;
        }
      }
      minimum_demerits = PENALTY_AWFUL;
      // end TeX@836;
    }
  }

  void deactivate_node(UniquePtr<State> &state, active_node *&r) {
    std::cout << "Deactivating.\n";
    // TeX@860
    if (r->prev == NULL) {
      // TeX@861
      active_node *new_head = r->link;
      r->unlink();
      r = new_head;
      if (r && r->type == DELTA) {
        active_width += r->d;
        cur_active_width = active_width;
        new_head = r->link;
        r->unlink();
        r = new_head;
      }
      active.head = r;
      // End TeX@861
    } else {

      active_node *prev_r = r->prev;
      r->unlink();
      delete r;
      r = prev_r->link;
      if (prev_r->type == DELTA) {
        if (!r) {
          cur_active_width -= prev_r->d;
          prev_r->unlink();
          delete prev_r;
        } else if (r->type == DELTA) {
          cur_active_width += r->d;
          prev_r->d += r->d;
          r->unlink();
          delete r;
          r = prev_r;
        }
      }
    }
    // end TeX@860
  }

  void break_line(UniquePtr<State> &state) {
    RenderState &render = state->render();
    assert(render.mode() == HMODE && "Attempted to break paragraph in wrong mode.");
    head = render.head();
    tail = render.tail();
    render.pop();
    render.set_mode(VMODE);
    if (!head)
      return;
    assert(tail && "Found NULL tail; tail should never be NULL when head is non-null.");

    if (tail->type == GLUE_NODE) {
      tail->type = PENALTY_NODE;
      tail->penalty = PENALTY_BREAK;
    } else {
      tail->link = RenderNode::new_penalty(PENALTY_BREAK);
      tail = tail->link;
      tail->link = NULL;
    }

    do {
      // insert an active node at the beginning of the paragraph.
      active_node *new_node = new active_node;
      new_node->type = ACTIVE;
      new_node->passive = NULL;
      new_node->fitness = DECENT_FIT;
      new_node->demerits = 0;
      active.append(new_node);
      active_width = background;


      cur_p = head;
      prev_p = NULL;

      while (cur_p) {
        switch (cur_p->type) {
          case GLUE_NODE: {
            if (cur_p->glue.shrink_order != GLUE_NORMAL)
              throw new GenericDiag("Encountered infinite shrinkage in HMODE.",
                                    DIAG_RENDER_ERR, BLAME_HERE);
            if (prev_p && prev_p->type != GLUE_NODE) {
              try_break(state, 0);
            }
            active_width[0] += cur_p->glue.width;
            active_width[1+cur_p->glue.stretch_order] += cur_p->glue.stretch;
            active_width[5] += cur_p->glue.shrink;
            break;
          }
          case RULE_NODE:
          case HBOX_NODE:
          case VBOX_NODE:
          case CHAR_NODE:
          case LIG_NODE:
          case KERN_NODE: {
            active_width[0] += cur_p->width(state);
            break;
          }
          case PENALTY_NODE: {
            print_active_nodes(state);
            std::cout << "Penalty node.\n";
            try_break(state, cur_p->penalty);
            print_active_nodes(state);
            break;
          }
          default:
            throw new GenericDiag("Unknown node type encountered in line break"
                                  "ing algorithm.", DIAG_RENDER_ERR, BLAME_HERE);
        }
        prev_p = cur_p;
        cur_p = prev_p->link;
      }
      if (!active.head) {
        assert(!final_pass);
        final_pass = true;
        continue;
      }
      break;
    } while(true);

    if (active.head) {
      active_node *r = active.head;
      int32_t fewest_demerits = PENALTY_AWFUL;
      while (r) {
        if (r->type == ACTIVE && r->demerits <= fewest_demerits) {
          best_bet = r;
          fewest_demerits = r->demerits;
        }
        r = r->link;
      }
    }

    post_line_break(state);
  }

  void try_break(UniquePtr<State> &state, int32_t penalty) {
    no_break_yet = true;
    if (penalty >= PENALTY_INF)
      return;
    if (penalty < PENALTY_BREAK)
      penalty = PENALTY_BREAK;

    cur_active_width = active_width;
    active_node *r = active.head;

    int32_t old_line = -1;
 
    while (true) {
      if (penalty == PENALTY_BREAK) {
        std::cout << "\t";
        if (!r)
          std::cout << "NULL\n";
        else if (r->type == ACTIVE)
          std::cout << "\tACTIVE\n";
        else
          std::cout << "\tDELTA\n";
      }
      // TeX@832
      if (r && r->type == DELTA) {
        cur_active_width += r->d;
        r = r->link;
        continue;
      }
      // End TeX@832

      // TeX@835

      int32_t line;
      if (!r) {
        add_nodes_if_necessary(state, r);
        return;
      } else  {
        line = r->line_number;
        if (line > old_line) {
          add_nodes_if_necessary(state, r);
          old_line = line;
        }
      }
      // TeX@850
      // Parshape/hanging indentation ignored for now. line_width is set in
      // constructor.
      // End TeX@850
      // End TeX@835

      // TeX@851

      bool artificial_demerits = false;
      sp shortfall = line_width - cur_active_width[0];
      std::cout << "Line width: " << line_width.string()
                << ", cur_active_width: " << cur_active_width[0].string()
                << ", shortfall: " << shortfall.string()
                << std::endl;
      int32_t bad = 0;
      if (shortfall > 0) {
        // TeX@852
        // if we have infinite glue
        if (cur_active_width[DELTA_FIL] != 0
            || cur_active_width[DELTA_FILL] != 0
            || cur_active_width[DELTA_FILLL] != 0) {
          bad = 0;
          fit_class = DECENT_FIT;
        } else  {
          if (shortfall > 7230584
              && cur_active_width[DELTA_NORMAL] < 1663497) {
              bad = PENALTY_INF;
              fit_class = VERY_LOOSE_FIT;
          } else {
            bad = badness(shortfall, cur_active_width[DELTA_NORMAL]);
            if (bad > 12) {
              if (bad > 99)
                fit_class = VERY_LOOSE_FIT;
              else
                fit_class = LOOSE_FIT;
            } else
            fit_class = DECENT_FIT;
          }
        }
        // end TeX@852
      } else {
        // TeX@853
        // need to shrink.
        if (-shortfall > cur_active_width[DELTA_SHRINK])
          bad = PENALTY_INF + 1;
        else
          bad = badness(-shortfall, cur_active_width[DELTA_SHRINK]);
        if (bad > 12)
          fit_class = TIGHT_FIT;
        else
          fit_class = DECENT_FIT;
        // end TeX@853
      }

      bool deactivate = false;
      if (bad > PENALTY_INF || penalty == PENALTY_BREAK) {
                // TeX@854
        if (final_pass
            && minimum_demerits == PENALTY_AWFUL
            && !r->link
            && !r->prev)
          artificial_demerits = true;
        else {
          if (bad > threshold) {
            deactivate_node(state, r);
            continue;
          }
          // deactivate, but still consider place for new node.
          deactivate = true;
        }
        // end TeX@854
      } else {
        if (bad > threshold) {
         r = r->link;
         continue;
        }
      }
      // TeX@855
      int32_t demerits;
      if (artificial_demerits)
        demerits = 0;
      else {
        // TeX@859
        demerits = line_penalty + bad;
        if (abs(demerits) > PENALTY_INF)
          demerits = PENALTY_AWFUL;
        else
          demerits *= demerits;
        if (penalty) {
          if (penalty > 0)
            demerits += penalty * penalty;
          else if (penalty > PENALTY_BREAK)
            demerits -= penalty * penalty;
        }
        if (abs(fit_class - r->fitness) > 1)
          demerits += state->eqtb()[ADJ_DEMERITS_CODE].i64;
        // End TeX@859
      }
      demerits += r->demerits;
      if (demerits <= minimal_demerits[fit_class]) {
        minimal_demerits[fit_class] = demerits;
        best_place[fit_class] = r->passive;
        best_place_line[fit_class] = line;
        if (demerits < minimum_demerits)
          minimum_demerits = demerits;
      }
      // end TeX@855
      if (deactivate) {
        deactivate_node(state, r);
        continue;
      }

      // End TeX@851
      r = r->link;
    }
  }

  void post_line_break(UniquePtr<State> &state) {

    // TeX@878
    passive_node *q = best_bet->passive;
    passive_node *cur_passive = NULL;
    while (q) {
      passive_node *r = q;
      q = r->parent;
      r->link = cur_passive;
      cur_passive = r;
    }
    // End TeX@878

    while (cur_passive) {
      // TeX@880
      // TeX@881
      RenderNode *break_point = cur_passive->break_point;
      if (break_point) {
        break_point->type = GLUE_NODE;
        break_point->glue = skip_glue(right_skip);
      } else {
        break_point = head;
        while (break_point->link)
          break_point = break_point->link;
        glue_node rs_glue = skip_glue(right_skip);
        RenderNode *glue = RenderNode::new_glue(rs_glue);
        break_point->link = glue;
        glue->link = NULL;
      }
      // End TeX@881

      // TeX@887
      // Detach the list.
      RenderNode *line_list = head;
      head = break_point->link;
      break_point->link = NULL;
      if (left_skip != 0) {

        glue_node ls_glue = skip_glue(left_skip);
        RenderNode *glue = RenderNode::new_glue(ls_glue);
        glue->link = line_list;
        line_list = glue;
      }
      // End TeX@887

      // TeX@889
      std::cout << "appending line." << std::endl;
      state->render().append(hpack(state, line_list, line_width, EXACTLY));
      // End TeX@889

      // End TeX@880
      cur_passive = cur_passive->link;
      if (cur_passive) {
        // TeX@879
        RenderNode *r = head;
        while (r && r != cur_passive->break_point && r->type == GLUE_NODE) {
          RenderNode *next = r->link;
          delete r;
          r = next;
        }
        if (r != head)
          head = r;
        // End TeX@879
      }
    }

  }

public:
  LineBreaker(UniquePtr<State> &state) {
    active_width.set_all(scaled(0));
    cur_active_width.set_all(scaled(0));
    background.set_all(scaled(0));
    break_width.set_all(scaled(0));
    active = active_list();

    left_skip = state->eqtb()[LEFT_SKIP_CODE].scaled;
    right_skip = state->eqtb()[RIGHT_SKIP_CODE].scaled;
    line_width = state->eqtb()[HSIZE_CODE].scaled;
    background[DELTA_WIDTH] = left_skip + right_skip;

    // TeX@834
    minimum_demerits = PENALTY_AWFUL;
    for (unsigned i = 0; i < MAX_FITNESS; i++) {
      minimal_demerits[i] = PENALTY_AWFUL;
    }
    // end TeX@834

    final_pass = false;
    threshold = state->eqtb()[PRETOLERANCE_CODE].i64;
    line_penalty = state->eqtb()[LINE_PENALTY_CODE].i64;

    passive = NULL;
    head = tail = NULL;
    best_bet = NULL;
  }
};

}

using namespace tex;

void tex::line_break(UniquePtr<State> &state) {
  LineBreaker breaker(state);
  breaker.break_line(state);
}