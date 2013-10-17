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

#ifndef __INCLUDE_IO_BLAMESOURCE_H__
#define __INCLUDE_IO_BLAMESOURCE_H__

#include <cassert>

#include <string>

/** A simple macro to generate a BlameSource with the source information. */
#define BLAME_HERE \
  BlameSource(__FILE__, __LINE__, __LINE__, 0, 0)

namespace tex {


/** A simple class holding line, column, file, and extent information. */
class BlameSource {
private:
  const char *input_name;
  uint32_t line_start, line_end;
  uint32_t col_start, col_end;
  char *str;
public:
  /** The default constructor initializes everything to NULL. */
  BlameSource() : input_name(NULL), line_start(0), line_end(0), col_start(0), col_end(0), str(NULL) {}
  /** The true constructor.
   * @param input The file name of the text file.
   * @param lineStart The starting line.
   * @param lineEnd The ending line; must be greater than or equal to lineStart.
   * @param colStart The starting column.
   * @param colEnd The ending column; must be greater than or equal to colStart.
   */
  BlameSource(const char *input, uint32_t lineStart, uint32_t lineEnd, uint32_t colStart,
              uint32_t colEnd);

  /** Returns a formatted string in the format of "name:line[-line]:col[-col]" */
  char *string() const {
    assert(str && "Attempted to return NULL string.");
    return str;
  }
};

}

#endif  // __INCLUDE_IO_BLAMESOURCE_H__