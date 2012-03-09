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

#include <cstdio>

#include <Parse/TokenInputStream.h>
#include <State/State.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

int main(int argc, char **argv) {
  UniquePtr<State> state;
  State::init(state);
  UniquePtr<TokenInputStream> cmd_input;
  TokenInputStream::init_from_file("test.tex", new ASCIICodec(), cmd_input);
  Token result;
  while (!cmd_input->consume_token(state, result)) {
    printf("[%d,0x%016llX]", result.cmd, result.i64);
  }
  return 0;
}