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

#include <IO/CodecOutputStream.h>

using namespace tex;

Diag *CodecOutputStream::init_with_file(const char *filename, const Codec *codec, UniquePtr<CodecOutputStream> &result) {
  assert(filename && "Got passed NULL filename.");
  assert(codec && "Got passed NULL codec.");

  FILE *output = NULL;
  char *path = (char*)filename;
  if (path[0] == '-' && path[1] == '\0') {
    path = (char*)"/dev/stdout";
  }
  output = fopen(path, "w");
  if (!output)
    return new GenericDiag("Unable to open stream for writing.", DIAG_FILE_OPEN_ERR, BLAME_HERE);
  
  CodecOutputStream *stream = new CodecOutputStream();
  stream->output_stream.reset(output);
  stream->codec = codec;
  result.reset(stream);
  return NULL;
}

Diag *CodecOutputStream::write_unichar(unichar out) {
  EncodedCharacter ec = codec->encode(out);
  if (fwrite(ec.encoded, 1, ec.length, output_stream.get()) != ec.length)
    return new GenericDiag("Could not write entire character to stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);
  return NULL;
}

CodecOutputStream::~CodecOutputStream(void) {
  if (output_stream)
    fclose(output_stream.get());
  output_stream.take();
}