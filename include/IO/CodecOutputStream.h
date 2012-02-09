#ifndef __INCLUDE_CODECOUTPUTSTREAM_H__
#define __INCLUDE_CODECOUTPUTSTREAM_H__

#include <cstdio>

#include <Diag/Diag.h>
#include <Unicode/Codec.h>
#include <Util/UniquePtr.h>

namespace tex {

class CodecOutputStream {
private:
  CodecOutputStream(const CodecOutputStream&);
  CodecOutputStream &operator=(const CodecOutputStream&);
  CodecOutputStream() : output_stream(), codec(NULL) {}

  UniquePtr<FILE> output_stream;
  const Codec *codec;

public:
  ~CodecOutputStream(void);
  static Diag *init_with_file(const char *path, const Codec *codec, UniquePtr<CodecOutputStream> &result);
  
  Diag *write_unichar(unichar out);
  
};

}

#endif  // __INCLUDE_CODEOUTPUTSTREAM_H__