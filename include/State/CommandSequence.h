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

#ifndef __INCLUDE_STATE_COMMANDSEQUENCE_H__
#define __INCLUDE_STATE_COMMANDSEQUENCE_H__

#include <State/Word.h>
#include <Unicode/UString.h>

namespace tex {

/** A simple typedef to allow easy changing of CommandCodes at a later point. */
typedef uint32_t CommandCode;

/** Used to store command sequences; e.g. "\end", "\par" */
typedef struct {
  /** The name of the command sequence. */
  UString string;
  /** The command code classifying the command sequence. */
  CommandCode cmd;
  /** an operand to the sequence. */
  word operand;
} CommandSequence;

}

#endif  // __INCLUDE_STATE_COMMANDSEQUENCE_H__