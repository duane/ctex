/*****************************************************************************
*  Copyright (c) 2012 Duane Ryan Bailey                                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*****************************************************************************/

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