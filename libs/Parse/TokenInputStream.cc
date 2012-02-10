#include <IO/CodecOutputStream.h>
#include <Parse/TokenInputStream.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

int TokenInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<TokenInputStream> &result) {
  TokenInputStream *stream = new TokenInputStream();
  int status = CodecInputStream::init_from_file(path, codec, stream->input_stream);
  if (status)
    return status;
  result.reset(stream);
  return 0;
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

// converts line feeds to carriage returns.
int TokenInputStream::read_converted_char(State &state, unichar &uc) {
  if (read_translated_char(state, uc))
    return -1;
  if (uc == '\n')
    uc = '\r';
  return 0;
}

int TokenInputStream::read_command_sequence(State &state, UString &result) {
  MutableUString string = MutableUString();
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
  size_t line = input_stream->line();
  size_t col = input_stream->col();
  const char *name = input_stream->name();
  
  unichar uc;
  if (read_converted_char(state, uc)) {
    result.cmd = CC_EOF;  
    return NULL;
  }
  
  CommandCode ccode = state.catcode(uc);
  switch (ccode) {
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
      result.line = line;
      result.col = col;
      result.extent = input_stream->col() - col + 1;
      return NULL;
    }
    case CC_LETTER:
    case CC_OTHER_CHAR:
    case CC_CAR_RET:
    case CC_SPACER: {
      result.cmd = ccode;
      result.uc = uc;
      result.line = line;
      result.col = col;
      result.extent = 1;
      return NULL;
    }
    case CC_INVALID:
    default: {
      return new BlameSourceDiag("Found an invalid character.", DIAG_PARSE_ERR, BLAME_HERE, BlameSource(name, line, line, col, col));
    }
  }
  assert(false && "Reached unreachable code! Please fix.");
  return NULL;
}