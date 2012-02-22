#include <IO/BinaryOutputStream.h>

using namespace tex;

Diag *BinaryOutputStream::init_with_file(const char *path, UniquePtr<BinaryOutputStream> &result) {
  FILE *output = fopen(path, "w");
  if (!output) {
    return new GenericDiag("Unable to open file.", DIAG_FILE_OPEN_ERR, BLAME_HERE);
  }
  
  BinaryOutputStream *stream = new BinaryOutputStream();
  stream->output = output;
  result.reset(stream);
  return NULL;
}

BinaryOutputStream::~BinaryOutputStream(void) {
  if (output)
    fclose(output);
}

Diag *BinaryOutputStream::write_uint8(uint8_t byte) {
  if (fwrite(&byte, 1, 1, output) != 1)
    return new GenericDiag("Unable to write byte to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);
 
  curr_offset += 1;
  return NULL;
}

Diag *BinaryOutputStream::write_uint16(uint16_t byte16) {
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_16(byte16);
  
  if (fwrite(&byte16, 2, 1, output) != 1)
    return new GenericDiag("Unable to write bytes to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);

  
  curr_offset += 2;
  return NULL;
}

Diag *BinaryOutputStream::write_uint24(uint32_t byte24) {
  uint8_t bytes[3];
  if (curr_endian == ENDIAN_BIG) {
    bytes[0] = byte24 >> 16;
    bytes[1] = byte24 >> 8;
    bytes[2] = byte24;
  } else {
    bytes[2] = byte24 >> 16;
    bytes[1] = byte24 >> 8;
    bytes[0] = byte24;
  }
  
  if (fwrite(bytes, 3, 1, output) != 1)
    return new GenericDiag("Unable to write bytes to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);
  

  curr_offset += 3;
  return NULL;
}

Diag *BinaryOutputStream::write_uint32(uint32_t byte32) {
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_32(byte32);
  
  if (fwrite(&byte32, 4, 1, output) != 1)
    return new GenericDiag("Unable to write bytes to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);

  
  curr_offset += 4;
  return NULL;
}

Diag *BinaryOutputStream::write_uint64(uint64_t byte64) {
  if (curr_endian != ENDIAN_HOST)
    swap_bytes_64(byte64);
  
  if (fwrite(&byte64, 8, 1, output) != 1)
    return new GenericDiag("Unable to write bytes to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);


  curr_offset += 8;
  return NULL;
}

Diag *BinaryOutputStream::write_bytes(uint8_t *bytes, size_t len) {
  if (fwrite(bytes, 1, len, output) != len)
    return new GenericDiag("Unable to write bytes to output stream.", DIAG_FILE_WRITE_ERR, BLAME_HERE);

  curr_offset += len;
  return NULL;
}
