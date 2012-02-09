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
  CodecInputStream() : buf(), index(0), codec(NULL), peeked(false) {}
  
  UniquePtr<ByteBuffer> buf;
  size_t index;
  const Codec *codec;
  DecodedCharacter peek;
  bool peeked;
public:
  
  int consume_char(unichar &read);
  int peek_char(unichar &read);
  
  static int init_from_file(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result);
};
}  // namespace tex

#endif  // __INCLUDE_IO_CODECINPUTSTREAM_H__