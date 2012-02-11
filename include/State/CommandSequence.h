#ifndef __INCLUDE_STATE_COMMANDSEQUENCE_H__
#define __INCLUDE_STATE_COMMANDSEQUENCE_H__

#include <Unicode/UString.h>

namespace tex {

/** A simple typedef to allow easy changing of CommandCodes at a later point. */
typedef uint32_t CommandCode;

/** Used to store command sequences; e.g. "\end", "\par" */
typedef struct {
  /** The name of the command sequence. */
  UString string;
  /** The command code classifying the command sequence. */
  CommandCode cs;
} CommandSequence;

}

#endif  // __INCLUDE_STATE_COMMANDSEQUENCE_H__