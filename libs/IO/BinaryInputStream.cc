#include <IO/BinaryInputStream.h>

using namespace tex;

Diag *BinaryInputStream::init_from_file(const char *path, UniquePtr<BinaryInputStream> &result) {
  BinaryInputStream *stream = new BinaryInputStream();
  Diag *diag;
  if ((diag = ByteBuffer::init_from_file(path, stream->buf))) {
    delete stream;
    return diag;
  }
  result.reset(stream);
  return NULL;
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