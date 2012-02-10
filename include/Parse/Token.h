#ifndef __INCLUDE_PARSE_TOKEN_H__
#define __INCLUDE_PARSE_TOKEN_H__

#include <State/CommandSequence.h>

namespace tex {

typedef struct {
  CommandCode cmd;
  size_t line, col, extent;
  union {
    unichar uc;
    UString *string;
  };
} Token;

}

#endif  // __INCLUDE_PARSE_TOKEN_H__