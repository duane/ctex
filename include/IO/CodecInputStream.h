#ifndef __INCLUDE_IO_CODECINPUTSTREAM_H__
#define __INCLUDE_IO_CODECINPUTSTREAM_H__

#include <IO/CodecInputStream.h>
#include <Unicode/Codec.h>
#include <Util/ByteBuffer.h>

namespace tex {
class CodecInputStream {
private:
  // Disallow copy constructor/copy assignment
  CodecInputStream(const CodecInputStream&);
  CodecInputStream & operator=(const CodecInputStream &);
  CodecInputStream() : buf(), index(0), codec(NULL), peeked(false), cur_line(0), cur_col(0), stream_name(NULL) {}
  
  UniquePtr<ByteBuffer> buf;
  size_t index;
  const Codec *codec;
  DecodedCharacter peek;
  bool peeked;
  size_t cur_line, cur_col;
  const char *stream_name;
public:
  
  int consume_char(unichar &read);
  int peek_char(unichar &read);
  
  size_t line() const {
    return cur_line;
  }
  
  size_t col() const {
    return cur_col;
  }
  
  const char *name() const {
    return stream_name;
  }
  
  static int init_from_file(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result);
};
}  // namespace tex

#endif  // __INCLUDE_IO_CODECINPUTSTREAM_H__