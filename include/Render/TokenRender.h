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
