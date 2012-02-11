#ifndef __INCLUDE_IO_CODECINPUTSTREAM_H__
#define __INCLUDE_IO_CODECINPUTSTREAM_H__

#include <IO/CodecInputStream.h>
#include <Unicode/Codec.h>
#include <Util/ByteBuffer.h>

namespace tex {
/** An input stream which decodes input data into UTF-32 as requested. */
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
  
  /**
   * Reads a single character and advances the stream.
   * @param read Where the resulting character is stored
   * @return If EOF is reached, -1 is returned. Otherwise, 0 is returned.
   */
  int consume_char(unichar &read);
  
  /**
   * Looks at next character in input and caches it for future peaks and consumes.
   * @param read Where the resulting character is stored.
   * @return If EOF is reached, -1 is returned. Otherwise, 0 is returned.
   */
  int peek_char(unichar &read);
  
  /** Returns the current line of the stream. */
  size_t line() const {
    return cur_line;
  }
  
  /** Returns the current column of the stream. */
  size_t col() const {
    return cur_col;
  }
  
  /** Returns the name of the string as a constant C-string. */
  const char *name() const {
    return stream_name;
  }
  
  /** Initializes a stream from a path.
   * This is the only valid way to initialize a CodecInputStream.
   * @param path A constant C-string which follows the sematics of ByteBuffer::init_from_file().
   * @param codec A pointer to a valid Codec which will be used for decoding input.
   * @param result Where the resulting CodecInputStream is stored.
   * @return Zero on success, non-zero on failure.
   */
  static int init_from_file(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result);
};
}  // namespace tex

#endif  // __INCLUDE_IO_CODECINPUTSTREAM_H__