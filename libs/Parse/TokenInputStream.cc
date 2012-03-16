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

#include <IO/CodecOutputStream.h>
#include <Parse/TokenInputStream.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

void TokenInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result) {
  UniquePtr<TokenInputStream> stream;
  stream.reset(new TokenInputStream());
  CodecInputStream::init_from_file(path, codec, stream->input_stream);
  result.reset(stream.take());
}

inline unsigned hex_uchar_to_uint(unichar hex) {
  if (hex < '9')
    return hex - '0';
  return hex - 'a' + 10;
}

inline unichar hex_uchars_to_uchar(unichar c, unichar cc) {
  return hex_uchar_to_uint(c) * 16 + hex_uchar_to_uint(cc);
}

inline bool is_hex(unichar uc) {
  if (uc >= '0' && uc <= '9')
    return true;
  if (uc >= 'a' && uc <= 'f')
    return true;
  return false;
}

// Handles conversions from hex characters, per TeX@252
int TokenInputStream::read_translated_char(UniquePtr<State> &state, unichar &uc) {
  // first, read the raw character.
  unichar raw, raw2;
  if (input_stream->consume_char(raw))
    return -1;
  if (raw == '^') {
    if (input_stream->peek_char(raw2)) {
      return -1;
    }
    if (raw2 == '^') {
      // we've found an "expanded character"
      input_stream->consume_char(raw2);
      unichar c, cc;
      if (input_stream->peek_char(c))
        return -1;
      if (c < 128) {
        input_stream->consume_char(c);
        if (is_hex(c)) {
          input_stream->peek_char(cc);
          if (is_hex(cc)) {
            input_stream->consume_char(cc);
            uc = hex_uchars_to_uchar(c, cc);
            return 0;
          }
        }
        if (c < 64)
          uc = c + 64;
        else
          uc = c - 64;
        return 0;
      }
    }
  } else {
    uc = raw;
    return 0;
  }
  return 0;
}

int TokenInputStream::read_command_sequence(UniquePtr<State> &state, UString &result) {
  MutableUString string;
  unichar uc;
  while (!input_stream->peek_char(uc)) {
    if (state->catcode(uc) == CC_LETTER) {
      input_stream->consume_char(uc);
      string.add(uc);
    } else {
      break;
    }
  }
  result = string;
  return 0;
}

#define EOF_ERROR() \
  new GenericDiag("Unexpected EOF! Perhaps you forgot \\end?",\
                  DIAG_PARSE_ERR, BLAME_HERE);

int TokenInputStream::peek_token(UniquePtr<State> &state, Token &result) {
  assert(input_stream && "Attempted to read from a NULL stream.");
  
  // If we've already peeked at this token, return it.
  if (peeked) {
    result = peek_tok;
    return 0;
  }


  // first, record line/col of at start of token.
  peek_tok.line = input_stream->line();
  peek_tok.col = input_stream->col();
  peek_tok.extent = 1;
  const char *name = input_stream->name();
  
  if (read_translated_char(state, peek_tok.uc))
    throw EOF_ERROR();
    
  peek_tok.cmd = state->catcode(peek_tok.uc);
  switch (peek_tok.cmd) {
    case CC_IGNORE: {
      // do nothing. Tail call into the function to get next token.
      return peek_token(state, result);
    }
    case CC_ESCAPE: {
      UString *string = new UString();
      size_t cur_line = input_stream->line();
      size_t cur_col = input_stream->col();
      if (read_command_sequence(state, *string)) {
        throw new BlameSourceDiag("Error parsing command sequence.", DIAG_PARSE_ERR, BLAME_HERE, BlameSource(name, cur_line, cur_line, cur_col, cur_col));
      }

      // now look up CS in table
      CommandSequence *cs = state->get(*string);
      if (!cs)
        throw new BlameSourceDiag("Command sequence not found!.",
                                  DIAG_PARSE_ERR, BLAME_HERE,
                                  BlameSource(name, cur_line, cur_line,
                                              cur_col, input_stream->col()));

      peek_tok.cmd = cs->cmd;
      peek_tok.cs = cs;
      peek_tok.extent = input_stream->col() - peek_tok.col + 1;
      break;
    }
    case CC_LETTER:
    case CC_OTHER_CHAR: {
      parser_state = STATE_MIDLINE;
      break;
    }
    case CC_LBRACE:
    case CC_RBRACE: {
      break;
    }
    case CC_SPACER: {
      if (parser_state == STATE_SKIP_SPACES || parser_state == STATE_NEWLINE) {
        return peek_token(state, result);
      }
      parser_state = STATE_SKIP_SPACES;
      break;
    }
    case CC_CAR_RET: {
      uint32_t p_state = parser_state;
      parser_state = STATE_NEWLINE;
      if (p_state == STATE_NEWLINE) {
        CommandSequence *par = state->get(UString("par"));
        assert(par && "\\par is missing from CS table.");
        peek_tok.cmd = CC_PAR_END;
        peek_tok.cs = par;
        break;
      } else if (p_state == STATE_SKIP_SPACES) {
        return peek_token(state, result);
      } else if (p_state == STATE_MIDLINE) {
        peek_tok.cmd = CC_SPACER;
        peek_tok.uc = ' ';
        break;
      }
    }
    case CC_COMMENT: {
      // eat up characters until we run across a newline or EOF.
      parser_state = STATE_NEWLINE;
      unichar uc;
      while (!input_stream->consume_char(uc)) {
        if (state->catcode(uc) == CC_CAR_RET) // success!
          return peek_token(state, result);
      }
      // Whoops, reached EOF.
      throw EOF_ERROR();
    }
    case CC_INVALID:
    default: {
      
      throw new BlameSourceDiag("Found an invalid character.", DIAG_PARSE_ERR, BLAME_HERE, BlameSource(name, peek_tok.line, peek_tok.line, peek_tok.col, peek_tok.col));
    }
  }
  peeked = true;
  result = peek_tok;
  return 0;
}

int TokenInputStream::consume_token(UniquePtr<State> &state, Token &result) {
  int status = peek_token(state, result);
  peeked = false;
  return status;
}