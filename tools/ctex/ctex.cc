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
#include <Unicode/Codecs/ASCII.h>

#include <iostream>

using namespace tex;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "USAGE: ctex file.tex\n";
    return 1;
  }
  Path in_file = Path(argv[1]);
  if (in_file.file() == "") {
    std::cout << "Expected input file name!\n";
    return 1;
  }
  if (in_file.ext() != "tex") {
    std::cout << "Input file must have \".tex\" extension.\n";
    return 1;
  }
  Path out_file = in_file;
  out_file.set_ext("dvi");
  UniquePtr<State> state;
  UniquePtr<TokenRender> render;
  UniquePtr<DVI> output;
  Codec *codec = new ASCIICodec();

  try {
    State::init(state);
    TokenRender::init_from_file(in_file.full_path().c_str(), codec, render);
    DVI::init_with_file(out_file.full_path().c_str(), output);

    render->render_input(state);
    output->render(state);
  } catch (Diag *diag) {
    diag->print();
    return 1;
  }

  return 0;
}
