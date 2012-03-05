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