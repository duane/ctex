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
  uint32_t line;
  /** Column of beginning of token. */
  uint32_t col;
  
  /** Extent in columns of the token. */
  uint32_t extent;
  union {
    /** For most tokens, the single character contains the information needed. */
    unichar uc;
    /** For parsed integers. */
    int64_t i64;
    /** For command sequences, the string holds the command sequence given. */
    CommandSequence *cs;
  };
} Token;

}

#endif  // __INCLUDE_PARSE_TOKEN_H__