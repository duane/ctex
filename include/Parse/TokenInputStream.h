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

  TokenInputStream() : input_stream(), parser_state(STATE_NEWLINE),
                     peeked(false) {}
private:
  UniquePtr<CodecInputStream> input_stream;

  // Parser state emulates original tex parser state.
  unsigned parser_state;
  enum {
    STATE_NEWLINE,
    STATE_SKIP_SPACES,
    STATE_MIDLINE,
  };

  Token peek_tok;
  bool peeked;
private:
  // internal parsing methods
  int read_translated_char(UniquePtr<State> &state, unichar &uc);
  int read_command_sequence(UniquePtr<State> &state, UString &result);

public:
  /** 
   * Parses the file and produces tokens.
   * @param state The state of the tex file.
   * @param result Where the resulting token is stored. EOF is indicated by returning a token with the cmd type of CC_EOF
   * @return NULL on success, non-NULL diagnostic on error.
   */
  int consume_token(UniquePtr<State> &state, Token &result);
  
  /**
   * "Peeks" at the token without consuming it.
   * @param state The state of the tex file.
   * @param result Where the resulting token is stored. EOF is indicated by returning a token with the cmd type of CC_EOF
   * @return NULL on success, non-NULL diagnostic on error.
   */
   int peek_token(UniquePtr<State> &state, Token &result);

  /**
   * Initializes the stream from a  file.
   * @param path A constant C-String indicating the location of the file.
   * @param codec The codec used to decode the file.
   * @param result Where the resulting TokenInputStream is stored.
   * @return NULL on success, a non-NULL diagnostic on failure.
   */
  static void init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result);
};
}

#endif