#include <IO/CodecInputStream.h>

using namespace tex;

int CodecInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result) {
  assert(codec && "Was passed a NULL codec!");
  UniquePtr<ByteBuffer> buf;
  if (ByteBuffer::init_from_file(path, buf)) {
    return -1;
  }
  CodecInputStream *stream = new CodecInputStream();
  stream->codec = codec;
  stream->buf.reset(buf.take()); // copy the pointer safely.
  stream->stream_name = path;
  result.reset(stream);
  return 0;
}

int CodecInputStream::peek_char(unichar &read) {
  if (peeked) {
    read = peek.uchar;
    return 0;
  }
  if (index >= buf->size())
    return -1;
  peek = codec->decode(buf->begin(), index, buf->size());
  read = peek.uchar;
  peeked = true;
  return 0;
}

int CodecInputStream::consume_char(unichar &read) {
  if(peek_char(read))
    return -1;
  
  if (read == '\n' || read == '\n') {
    cur_line += 1;
    cur_col = 0;
  } else {
    cur_col += 1;
  }
  
  index += peek.length;
  peeked = false;
  return 0;
}