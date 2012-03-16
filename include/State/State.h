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

#ifndef __INCLUDE_STATE_STATE_H__
#define __INCLUDE_STATE_STATE_H__

#include <stdint.h>


#include <Render/RenderNode.h>
#include <Render/SimplePageBuilder.h>
#include <State/Codes.h>
#include <State/CommandSequence.h>
#include <State/EQTB.h>
#include <State/RenderState.h>
#include <tex/codes.h>
#include <Type/Font.h>
#include <Unicode/Unicode.h>
#include <Util/HashMap.h>
#include <Util/SmallVector.h>
#include <Util/UniquePtr.h>


namespace tex {

/** The default hash table size used to store CommandSequences. */
const uint32_t CS_TABLE_SIZE = 4098;

/** A helper datastructure forming the core of the hash table. */
struct CommandSequenceEntry {
  /** The stored CommandSequence. */
  CommandSequence cs;
  /** A pointer to the next entry in the bucket's linked list. */
  CommandSequenceEntry *next;
};

typedef EQTB<MAX_INTERNAL_CODE> tex_eqtb;

/** The kernel of the tex runtime.
 * This class holds all the state for the program used in parsing files.
 * It stores the category codes for each character.
 * It also holds a hash table with every command sequence, which is used an mutated during parsing.
 */
class State {

private:
  // Disallow copy/assign
  State(const State &);
  State &operator=(const State &);

    /** Initializes the state of the tex program to its default state. */
  State(void);

private:
  // the actual state.
  uint8_t ccode[128];
  
  // CS table state
  HashMap<UString, CommandSequence> cs_map;
  
  SmallVector<Font, 64> fonts;

  tex_eqtb the_eqtb;

  RenderState r_state;
  SimplePageBuilder p_builder;
public:
  /** @return The category code of the given character.*/
  uint8_t catcode(unichar uc) const {
    if (uc >= 128)
      return CC_INVALID;
    return ccode[uc];
  }

  Font &metrics(const uint32_t f) {
    return fonts[f];
  }

  ~State(void) {

  }

  /**
   * Sets or adds a command sequence to the hash table.
   * @param cs The CommandSequence to be added to the table.
   */
  void set(CommandSequence &cs) {
    cs_map.set(cs.string, cs);
  }
  
  /**
   * Fetches the CommandSequence for the given string.
   * @param string The string of the string to be looked up.
   * @return The CommandSequence found in the table, or NULL if nothing was found.
   */
  CommandSequence *get(UString string) {
    return cs_map.get(string);
  }
  
  /** Returns the number of command sequences currently in the table. */
  uint32_t cs_entries(void) const {
    return cs_map.entries();
  }

  uint32_t load_font(const char *font, const char *cs, int32_t at);

  RenderState &render(void) {
    return r_state;
  }

  tex_eqtb &eqtb(void) {
    return the_eqtb;
  }

  void primitive(const char *name, CommandCode cmd, word operand) {
    UString uname = UString(name);
    CommandSequence cs = {UString(uname), cmd, operand};
    cs_map.set(uname, cs);
  }

  SimplePageBuilder &builder(void) {
    return p_builder;
  }

  static void init(UniquePtr<State> &result);
};

}

#endif  // __INCLUDE_STATE_STATE_H__