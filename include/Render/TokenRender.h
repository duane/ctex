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

#ifndef __INCLUDE_RENDER_TOKENRENDER_H__
#define __INCLUDE_RENDER_TOKENRENDER_H__

#include <Parse/TokenInputStream.h>
#include <State/State.h>
#include <Unicode/Codec.h>

namespace tex {

class TokenRender {
private:
  // Disallow copy/assign.
  TokenRender(const TokenRender&);
  TokenRender &operator=(const TokenRender&);
  TokenRender(void) : input() {}

private:
  UniquePtr<TokenInputStream> input;

public:
  static void init_from_file(const char *path, const Codec *codec, UniquePtr<TokenRender> &result);

  void render_input(UniquePtr<State> &state);
};

}

#endif  // __INCLUDE_RENDER_TOKENRENDER_H__
