#ifndef __INCLUDE_STATE_STATE_H__
#define __INCLUDE_STATE_STATE_H__

#include <stdint.h>

#include <Unicode/Unicode.h>
#include <tex/codes.h>

namespace tex {

class State {
private:
  // Disallow copy/assign
  State(const State &);
  State &operator=(const State &);

private:
  // the actual state.
  uint8_t ccode[128];

public:
  uint8_t catcode(unichar uc) const {
    if (uc >= 128)
      return CC_INVALID;
    return ccode[uc];
  }
  
  State() {
    // first initialize ccode to all "CC_OTHER_CHAR" as the default code.
    for (unsigned i = 0; i < 128; i++) {
      ccode[i] = CC_OTHER_CHAR;
    }
    // now initialize letters.
    for (unsigned i = 0; i < 26; i++) {
      ccode['A' + i] = CC_LETTER;
      ccode['a' + i] = CC_LETTER;
    }
    
    // now misc. initializations.
    ccode[0x00] = CC_IGNORE;
    ccode[0x32] = CC_SPACER; // ' ' 
    ccode[0x5C] = CC_ESCAPE; // '\\'
    ccode[0x25] = CC_COMMENT; // '%'
    ccode[0x7F] = CC_INVALID;
    ccode[0x0D] = CC_CAR_RET; // '\r'
  }
};

}

#endif  // __INCLUDE_STATE_STATE_H__