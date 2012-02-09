#ifndef __INCLUDE_PARSE_COMMANDINPUTSTREAM_H__
#define __INCLUDE_PARSE_COMMANDINPUTSTREAM_H__


#include <Diag/Diag.h>
#include <IO/CodecInputStream.h>
#include <State/State.h>
#include <Util/UniquePtr.h>

namespace tex {
class CommandInputStream {
private:
  // Disallow copy/assign
  CommandInputStream(const CommandInputStream&);
  CommandInputStream &operator=(const CommandInputStream&);

  CommandInputStream() : input_stream() {}  
private:
  // Nothing but a wrapper around a CodecInputStream.
  UniquePtr<CodecInputStream> input_stream;

private:
  // internal parsing methods
  int read_translated_char(State &state, unichar &uc);
  int read_converted_char(State &state, unichar &uc);

public:
  Diag *consume_command(State &state, uint8_t &result);
  
  static int init_from_file(const char *path, const Codec *codec, UniquePtr<CommandInputStream> &result);
};
}

#endif