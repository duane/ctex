#ifndef __INCLUDE_PARSE_TokenINPUTSTREAM_H__
#define __INCLUDE_PARSE_TokenINPUTSTREAM_H__


#include <Diag/Diag.h>
#include <IO/CodecInputStream.h>
#include <State/State.h>
#include <Parse/Token.h>
#include <Util/UniquePtr.h>

namespace tex {
class TokenInputStream {
private:
  // Disallow copy/assign
  TokenInputStream(const TokenInputStream&);
  TokenInputStream &operator=(const TokenInputStream&);

  TokenInputStream() : input_stream(), parser_state(STATE_NEWLINE) {}
private:
  UniquePtr<CodecInputStream> input_stream;

  // Parser state emulates original tex parser state.
  unsigned parser_state;
  enum {
    STATE_NEWLINE,
    STATE_SKIP_SPACES,
    STATE_MIDLINE,
  };

private:
  // internal parsing methods
  int read_translated_char(State &state, unichar &uc);
  int read_command_sequence(State &state, UString &result);

public:
  Diag *consume_token(State &state, Token &result);
  
  static int init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result);
};
}

#endif