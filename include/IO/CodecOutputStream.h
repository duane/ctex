#ifndef __INCLUDE_CODECOUTPUTSTREAM_H__
#define __INCLUDE_CODECOUTPUTSTREAM_H__

#include <cstdio>

#include <Diag/Diag.h>
#include <Unicode/Codec.h>
#include <Util/UniquePtr.h>

namespace tex {

/** Allows writing UTF-32 to a file, encoded on the fly into another encoding. */
class CodecOutputStream {
private:
  CodecOutputStream(const CodecOutputStream&);
  CodecOutputStream &operator=(const CodecOutputStream&);
  CodecOutputStream() : output_stream(), codec(NULL) {}

  UniquePtr<FILE> output_stream;
  const Codec *codec;

public:
  /**
   *  Closes the stream.
   */
  ~CodecOutputStream(void);
  
  /**
   *  The only initializer; creates a new CodecOutputStream for the given file.
   *  @param path Either a valid path to an existing or new file, or "-" for stdout.
   *  @param codec The codec to be used to encode outgoing text.
   *  @param result Where the resulting CodecOutputStream is stored.
   *  @return NULL on success, non-NULL diagnostic on failure.
   */
  static Diag *init_with_file(const char *path, const Codec *codec, UniquePtr<CodecOutputStream> &result);
  
  /**
   * Writes a single character to the stream.
   * @param out The character to be written.
   * @return NULL on success, non-NULL on failure.
   */
  Diag *write_unichar(unichar out);
  
};

}

#endif  // __INCLUDE_CODEOUTPUTSTREAM_H__