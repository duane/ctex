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

#include <Output/DVI.h>
#include <State/State.h>
#include <Render/TokenRender.h>
#include <Unicode/Codecs/UTF8.h>

using namespace tex;

int main(int argc, char **argv) {
  
  UniquePtr<State> state;
  UniquePtr<TokenRender> render;
  UniquePtr<DVI> output;
  Codec *codec = new UTF8Codec();

  try {
    State::init(state);
    TokenRender::init_from_file("hello.tex", codec, render);
    DVI::init_with_file("hello.dvi", output);

    render->render_input(state);
    output->render(state);
  } catch (Diag *diag) {
    diag->print();
    return 1;
  }

  return 0;
}
