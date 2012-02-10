#include <cstdio>

#include <Parse/TokenInputStream.h>
#include <State/State.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

int main(int argc, char **argv) {
  State state;
  UniquePtr<TokenInputStream> cmd_input;
  if (TokenInputStream::init_from_file("test.tex", new ASCIICodec(), cmd_input)) {
    printf("PANIC.\n");
    return 1;
  }
  
  Diag *diag = NULL;
  Token result;
  while (!(diag = cmd_input->consume_token(state, result))) {
    if (result.cmd == CC_EOF)
      return 0;
    printf("[%d,", result.cmd);
    if (result.cmd == CC_CS_STRING)
      printf("%llX]", (uint64_t)result.string);
    else
      printf("%X]", result.uc);
  }
  diag->print();
  return 1;
}