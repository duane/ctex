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

#include <cstring>

#include <Unicode/UString.h>

using namespace tex;

UString::UString(const char *cstr) {
  assert(cstr && "Attempted to initialize UString with NULL C-string.");
  allocated = length = strlen(cstr);
  raw = new unichar[allocated];
  for (size_t i = 0; i < length; i++) {
    raw[i] = (unichar)cstr[i];
  }
  dirty_hash = true;
}
