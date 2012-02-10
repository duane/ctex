#ifndef __INCLUDE_STATE_STATE_H__
#define __INCLUDE_STATE_STATE_H__

#include <stdint.h>

#include <Unicode/Unicode.h>
#include <State/CommandSequence.h>
#include <tex/codes.h>


namespace tex {
const uint32_t CS_TABLE_SIZE = 4098;

struct CommandSequenceEntry {
  CommandSequence cs;
  CommandSequenceEntry *next;
};

class State {

private:
  // Disallow copy/assign
  State(const State &);
  State &operator=(const State &);

private:
  // the actual state.
  uint8_t ccode[128];

  // CS table state
  uint32_t cs_entries, cs_size;
  CommandSequenceEntry **cs_table;
  
private:
  // internal methods
  
  CommandSequenceEntry *find_cs(UString &string, CommandSequenceEntry ***reference) const;
public:
  uint8_t catcode(unichar uc) const {
    if (uc >= 128)
      return CC_INVALID;
    return ccode[uc];
  }
  
  State();
  void set(CommandSequence &cs);
  CommandSequence *get(UString &string);
};

}

#endif  // __INCLUDE_STATE_STATE_H__