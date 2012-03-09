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

#include <IO/BinaryInputStream.h>

using namespace tex;

void BinaryInputStream::init_from_file(const char *path, UniquePtr<BinaryInputStream> &result) {
  BinaryInputStream *stream = new BinaryInputStream();
  try {
    ByteBuffer::init_from_file(path, stream->buf);
  } catch (Diag *diag) {
    delete stream;
    throw diag;
  } 
  result.reset(stream);
}


int BinaryInputStream::read_uint8(uint8_t &result) {
  if (curr_offset >= buf->size())
    return -1;
  
  result = buf->get(curr_offset);
  curr_offset += 1;
  return 0;
}

int BinaryInputStream::read_uint16(uint16_t &result) {
  if ((curr_offset + 1) >= buf->size())
    return -1;
  
  buf->get_bytes(curr_offset, 2, (uint8_t*)&result);
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_16(result);
  curr_offset += 2;
  return 0;
}

int BinaryInputStream::read_uint32(uint32_t &result) {
  if ((curr_offset + 3) >= buf->size())
    return -1;

  buf->get_bytes(curr_offset, 4, (uint8_t*)&result);
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_32(result);
  curr_offset += 4;
  return 0;
}

int BinaryInputStream::read_uint64(uint64_t &result) {
  if ((curr_offset + 7) >= buf->size())
    return -1;
  
  buf->get_bytes(curr_offset, 8, (uint8_t*)&result);
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_64(result);
  curr_offset += 8;
  return 0;
}

int BinaryInputStream::read_bytes(uint8_t *dest, uint32_t size) {
  if ((curr_offset + size) >= buf->size())
    return -1;
  buf->get_bytes(curr_offset, size, dest);
  curr_offset += size;
  return 0;
}