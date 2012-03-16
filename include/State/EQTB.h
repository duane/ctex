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

#ifndef __INCLUDE_STATE_EQTB_H__
#define __INCLUDE_STATE_EQTB_H__

#include <State/Word.h>
#include <Util/SmallVector.h>

namespace tex {

template <uint32_t N>
class EQTB {
private:
  EQTB(const EQTB &);
  EQTB &operator=(const EQTB &);


  struct eqtb_entry {
    uint32_t code;
    uint32_t level;
    word value;
  };

  typedef SmallVector<eqtb_entry, 64> eqtb_level_frame;

private:
  word table[N];
  uint8_t levels[N];

  uint8_t cur_level;
  SmallVector<eqtb_level_frame, 256> save_stack;
  eqtb_level_frame cur_frame;

public:

  word &operator[](uint32_t code) const {
    assert(code < N && "Attempted to access value outside the bounds of equivalence table.");
    assert(levels[code] && "Attempted to read undefined value.");
    return table[code];
  }

  word &operator[](uint32_t code) {
    assert(code < N && "Attempted to get/set value outside the bounds of the equivalence table.");
    assert(levels[code] <= cur_level && "Found bad level in EQTB.");
    if (!levels[code] && cur_level == 1) {
      levels[code] = 1;
    } else if (levels[code] < cur_level) {
      // transition it to the new level.
      eqtb_entry entry;
      entry.level = levels[code];
      entry.value = table[code];
      entry.code = code;
      cur_frame.push(entry);
      levels[code] = cur_level;
    }
    return table[code];
  }

  EQTB(void) : cur_level(1) {
    bzero(levels, N);
  }

  void enter_grouping(void) {
    cur_level += 1;
    save_stack.push(cur_frame);
    cur_frame.reset();
  }

  void leave_grouping(void) {
    assert(cur_level > 1 && "Attempted to leave global group.");
    cur_level -= 1;
    for (unsigned i = 0; i < cur_frame.entries(); i++) {
      eqtb_entry entry = cur_frame[i];
      levels[entry.code] = entry.level;
      table[entry.code] = entry.value;
    }
    cur_frame = save_stack[save_stack.entries() - 1];
    save_stack.pop();
  }

  uint8_t level(void) const {
    return cur_level;
  }
};

}

#endif  // __INCLUDE_STATE_EQTB_H__