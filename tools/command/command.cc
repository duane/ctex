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
    printf("[%d,", result.cmd);
    if (result.cmd == CC_CS_STRING)
      printf("%llX]", (uint64_t)result.string);
    else
      printf("%X]", result.uc);
  }
  return 0;
}