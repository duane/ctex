#include <IO/CodecOutputStream.h>
#include <Parse/CommandInputStream.h>
#include <Unicode/Codecs/ASCII.h>

using namespace tex;

int CommandInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<CommandInputStream> &result) {
  CommandInputStream *stream = new CommandInputStream();
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
int CommandInputStream::read_translated_char(State &state, unichar &uc) {
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
int CommandInputStream::read_converted_char(State &state, unichar &uc) {
  if (read_translated_char(state, uc))
    return -1;
  if (uc == '\n')
    uc = '\r';
  return 0;
}

Diag *CommandInputStream::consume_command(State &state, uint8_t &result) {
  assert(input_stream && "Attempted to read from a NULL stream.");
  
  return NULL;
}