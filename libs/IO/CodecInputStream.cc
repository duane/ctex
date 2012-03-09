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

#include <IO/CodecInputStream.h>

using namespace tex;

void CodecInputStream::init_from_file(const char *path, const Codec *codec, UniquePtr<CodecInputStream> &result) {
  assert(codec && "Was passed a NULL codec!");
  UniquePtr<ByteBuffer> buf;
  ByteBuffer::init_from_file(path, buf);
  CodecInputStream *stream = new CodecInputStream();
  stream->codec = codec;
  stream->buf.reset(buf.take()); // copy the pointer safely.
  stream->stream_name = path;
  result.reset(stream);
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
  
  if (read == '\n' || read == '\r') {
    cur_line += 1;
    cur_col = 0;
  } else {
    cur_col += 1;
  }
  
  index += peek.length;
  peeked = false;
  return 0;
}