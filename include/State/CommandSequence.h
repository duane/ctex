#ifndef __INCLUDE_STATE_COMMANDSEQUENCE_H__
#define __INCLUDE_STATE_COMMANDSEQUENCE_H__

#include <Unicode/UString.h>

namespace tex {

typedef uint32_t CommandCode;

typedef struct {
  UString string;
  CommandCode cs;
} CommandSequence;

}

#endif  // __INCLUDE_STATE_COMMANDSEQUENCE_H__