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

#ifndef __INCLUDE_STATE_REGISTERFILE_H__
#define __INCLUDE_STATE_REGISTERFILE_H__

#include <State/Word.h>
#include <Util/SmallIntMap.h>

namespace tex {

template <uint32_t N>
class RegisterFile {
private:
  RegisterFile(const RegisterFile &);
  RegisterFile &operator=(const RegisterFile &);

private:
  SmallIntMap<word, N> file;

public:

  word &operator[](uint32_t code) {
    const word *res = file.get(code);
    if (!res) {
      file.set(code, (word){0});
      res = file.get(code);
    }
    // a dirty hack.
    word *mut = const_cast<word*>(res);
    return *mut;
  }

  RegisterFile(void) : file() {}

};

}

#endif  // __INCLUDE_STATE_REGISTERFILE_H__