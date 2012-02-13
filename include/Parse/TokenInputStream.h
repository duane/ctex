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
  /** 
   * Parses the file and produces tokens.
   * @param state The state of the tex file.
   * @param result Where the resulting token is stored. EOF is indicated by returning a token with the cmd type of CC_EOF
   * @return NULL on success, non-NULL diagnostic on error.
   */
  Diag *consume_token(State &state, Token &result);
  
  /**
   * Initializes the stream from a  file.
   * @param path A constant C-String indicating the location of the file.
   * @param codec The codec used to decode the file.
   * @param result Where the resulting TokenInputStream is stored.
   * @return Zero on success, non-zero on failure.
   */
  static Diag *init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result);
};
}

#endif