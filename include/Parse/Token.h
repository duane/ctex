#ifndef __INCLUDE_PARSE_TOKEN_H__
#define __INCLUDE_PARSE_TOKEN_H__

#include <State/CommandSequence.h>

namespace tex {

/**
 * A simple structure to store Tokens.
 */
typedef struct {
  /** Where the catcode/command type is stored. */
  CommandCode cmd;
  /** Line of token */
  size_t line;
  /** Column of beginning of token. */
  size_t col;
  
  /** Extent in columns of the token. */
  size_t extent;
  union {
    /** For most tokens, the single character contains the information needed. */
    unichar uc;
    /** For command sequences, the string holds the command sequence given. */
    UString *string;
  };
} Token;

}

#endif  // __INCLUDE_PARSE_TOKEN_H__