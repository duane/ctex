#include <Util/ByteBuffer.h>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>

using namespace tex;

void ByteBuffer::init_from_file(const char *path, UniquePtr<ByteBuffer> &result) {
  UniquePtr<ByteBuffer> buf;
  buf.reset(new ByteBuffer());
  FILE *file = fopen(path, "r");
  if (!file)
    throw new GenericDiag("Unable to open file.", DIAG_FILE_OPEN_ERR, BLAME_HERE);

  int fildes = fileno(file);
  struct stat fileStat;
  if (fstat(fildes, &fileStat))
    throw new GenericDiag("Unable to stat file.", DIAG_FILE_MISC_ERR, BLAME_HERE);
  size_t fileSize = fileStat.st_size;
  
  uint8_t *raw_buffer = new uint8_t[fileSize];

  if (fread(raw_buffer, 1, fileSize, file) != fileSize) {
    delete raw_buffer;
    throw new GenericDiag("Unable to read entire file.", DIAG_FILE_READ_ERR, BLAME_HERE);
  }

  if(fclose(file)) {
    delete raw_buffer;
    throw new GenericDiag("Unable to close file.", DIAG_FILE_MISC_ERR, BLAME_HERE);
  }

  buf->raw_buffer = raw_buffer;
  buf->buffer_size = fileSize;
  result.reset(buf.take());
}

uint8_t ByteBuffer::get(size_t offset) const {
  assert(offset < buffer_size && "Access byte outside bounds of buffer.");
  return raw_buffer[offset];
}

void ByteBuffer::get_bytes(size_t offset, size_t size, uint8_t *bytes) const {
  assert(bytes && "Got NULL pointer reference to result bytes.");
  assert(size && "Trying to copy zero bytes.");
  assert(offset + size <= buffer_size && "Attempted to copy bytes that exceed the bounds of the buffer.");
  assert(bytes + size <= raw_buffer || raw_buffer + buffer_size <= bytes && "Aliased pointers.");
  memcpy(bytes, raw_buffer + offset, size);
}
