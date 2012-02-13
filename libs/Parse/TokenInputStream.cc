#include <IO/CodecOutputStream.h>
#include <Parse/TokenInputStream.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

Diag *TokenInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result) {
  TokenInputStream *stream = new TokenInputStream();
  Diag *diag;
  if (!(diag = CodecInputStream::init_from_file(path, codec, stream->input_stream)))
    return diag;
  result.reset(stream);
  return NULL;
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
int TokenInputStream::read_translated_char(State &state, unichar &uc) {
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

int TokenInputStream::read_command_sequence(State &state, UString &result) {
  MutableUString string;
  unichar uc;
  while (!input_stream->peek_char(uc)) {
    if (state.catcode(uc) == CC_LETTER) {
      input_stream->consume_char(uc);
      string.add(uc);
    } else {
      break;
    }
  }
  result = string;
  return 0;
}

Diag *TokenInputStream::consume_token(State &state, Token &result) {
  assert(input_stream && "Attempted to read from a NULL stream.");
  
  // first, record line/col of at start of token.
  result.line = input_stream->line();
  result.col = input_stream->col();
  result.extent = 1;
  const char *name = input_stream->name();
  
  if (read_translated_char(state, result.uc)) {
    result.cmd = CC_EOF;  
    return NULL;
  }
  
  result.cmd = state.catcode(result.uc);
  switch (result.cmd) {
    case CC_IGNORE: {
      // do nothing. Tail call into the function to get next token.
      return consume_token(state, result);
    }
    case CC_ESCAPE: {
      UString *string = new UString();
      if (read_command_sequence(state, *string)) {
        size_t cur_line = input_stream->line();
        size_t cur_col = input_stream->col();
        return new BlameSourceDiag("Error parsing command sequence.", DIAG_PARSE_ERR, BLAME_HERE, BlameSource(name, cur_line, cur_line, cur_col, cur_col));
      }
      result.cmd = CC_CS_STRING;
      result.string = string;
      result.extent = input_stream->col() - result.col + 1;
      return NULL;
    }
    case CC_LETTER:
    case CC_OTHER_CHAR: {
      parser_state = STATE_MIDLINE;
      return NULL;
    }
    case CC_SPACER: {
      if (parser_state == STATE_SKIP_SPACES || parser_state == STATE_NEWLINE) {
        return consume_token(state, result);
      }
      parser_state = STATE_SKIP_SPACES;
      return NULL;
    }
    case CC_CAR_RET: {
      uint32_t p_state = parser_state;
      parser_state = STATE_NEWLINE;
      if (p_state == STATE_NEWLINE) {
        result.cmd = CC_CS_STRING;
        result.string = new UString("par");
        return NULL;
      } else if (p_state == STATE_SKIP_SPACES) {
        return consume_token(state, result);
      } else if (p_state == STATE_MIDLINE) {
        result.cmd = CC_SPACER;
        result.uc = ' ';
        return NULL;
      }
    }
    case CC_COMMENT: {
      // eat up characters until we run across a newline or EOF.
      parser_state = STATE_NEWLINE;
      unichar uc;
      while (!input_stream->consume_char(uc)) {
        if (state.catcode(uc) == CC_CAR_RET) // success!
          return consume_token(state, result);
      }
      // we've reached EOF.
      result.cmd = CC_EOF;
      return NULL;
    }
    case CC_INVALID:
    default: {
      return new BlameSourceDiag("Found an invalid character.", DIAG_PARSE_ERR, BLAME_HERE, BlameSource(name, result.line, result.line, result.col, result.col));
    }
  }
  assert(false && "Reached unreachable code! Please fix.");
  return NULL;
}