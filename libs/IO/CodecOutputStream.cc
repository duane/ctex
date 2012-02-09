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