#ifndef __INCLUDE_PARSE_TOKEN_H__
#define __INCLUDE_PARSE_TOKEN_H__

namespace tex {

typedef struct {
  uint32_t cmd;
  union {
    unichar uc;
    UString *string;
  };
} Token;

}

#endif  // __INCLUDE_PARSE_TOKEN_H__