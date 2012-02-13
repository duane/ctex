#ifndef __INCLUDE_ANCILLARY_BYTEBUFFER_H__
#define __INCLUDE_ANCILLARY_BYTEBUFFER_H__

#include <stdint.h>

#include <Diag/Diag.h>
#include <Util/UniquePtr.h>

namespace tex {

class ByteBuffer {
private:
  // disallow copy/assign
  ByteBuffer(const ByteBuffer&);
  ByteBuffer &operator=(const ByteBuffer&);
  ByteBuffer(void) : raw_buffer(NULL), buffer_size(0) {}

  uint8_t *raw_buffer;
  uint64_t buffer_size;

// Iterator interface
public:
  /** An iterator into the buffer. */
  typedef uint8_t *iterator;
  /** The beginning of the buffer. */
  iterator begin(void) const {
    return raw_buffer;
  }
  
  /** The byte after the end of the buffer. */
  iterator end(void) const {
    return raw_buffer + buffer_size;
  }

public:
  /**
   *  Initializes a ByteBuffer from a path. The file is copied into the buffer in its entirety, so be sure not to use this for large files.
   *  @param path The path to the file to be opened and read.
   *  @param result Where the resulting ByteBuffer is placed.
   *  @return Zero on success, non-zero on failure.
   */
  static Diag *init_from_file(const char *path, UniquePtr<ByteBuffer> &result);
  
  /**
   *  Fetches the byte at the offset.
   *  @param offset The index into the buffer. Must be less than the size of the buffer.
   *  @return The byte at the offset.
   */
  uint8_t get(size_t offset) const;
  
  /**
   *  Copies bytes from the byte buffer into given memory.
   *  @param offset The start of the source region of memory.
   *  @param size The size of the memory to be copied, in bytes. offset+size must not exceed the end of the buffer.
   *  @param bytes The destination memory.
   */
  void get_bytes(size_t offset, size_t size, uint8_t *bytes) const;
  
  /** Returns the size of the byte buffer in bytes. */
  inline uint64_t size(void) const {
    return buffer_size;
  }
  
  /** The deconstructor deallocates the buffer. */
  ~ByteBuffer(void) {
    delete[] raw_buffer;
  }
};

}  // namespace dviview

#endif  // __INCLUDE_ANCILLARY_BYTEBUFFER_H__
