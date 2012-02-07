#ifndef __INCLUDE_IO_CODECINPUTSTREAM_H__
#define __INCLUDE_IO_CODECINPUTSTREAM_H__

#include <Unicode/Codec.h>
#include <Util/ByteBuffer.h>

namespace tex {
class CodecInputStream {
private:
  // Disallow copy constructor/copy assignment
  CodecInputStream(const CodecInputStream&);
  CodecInputStream & operator=(const CodecInputStream &);
  CodecInputStream() : buf(), index(0), codec(NULL) {}
  
  UniquePtr<ByteBuffer> buf;
  size_t index;
  const Codec *codec;
public:
  
  int consumeChar(unichar &read);
  
  static int initFromFile(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result);
};
}  // namespace tex

#endif  // __INCLUDE_IO_CODECINPUTSTREAM_H__