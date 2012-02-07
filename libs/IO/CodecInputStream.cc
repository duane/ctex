#include <IO/CodecInputStream.h>

using namespace tex;

int CodecInputStream::initFromFile(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result) {
  assert(codec && "Was passed a NULL codec!");
  UniquePtr<ByteBuffer> buf;
  if (ByteBuffer::initFromFile(path, buf) == 0) {
    return -1;
  }
  CodecInputStream *stream = new CodecInputStream();
  stream->codec = codec;
  stream->buf.reset(buf.take()); // copy the pointer safely.
  result.reset(stream);
  return 0;
}

int CodecInputStream::consumeChar(unichar &read) {
  if (index >= buf->size()) {
    return -1;
  }
  DecodedCharacter dc = codec->decode(buf->begin(), index, buf->size() - index);
  index += dc.length;
  read = dc.uchar;
  return 0;
}