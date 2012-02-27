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
