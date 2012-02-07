#ifndef __INCLUDE_ANCILLARY_BYTEBUFFER_H__
#define __INCLUDE_ANCILLARY_BYTEBUFFER_H__

//#include <stddef.h>
#include <stdint.h>
#include <Util/UniquePtr.h>

namespace tex {

class ByteBuffer {
  private:
    // disallow copy/assign
    ByteBuffer(const ByteBuffer&);
    ByteBuffer &operator=(const ByteBuffer&);

    uint8_t *rawBuffer;
    uint64_t bufferSize;

  // Iterator interface
  public:
    typedef uint8_t *iterator;
    iterator begin() const {
      return rawBuffer;
    }
    
    iterator end() const {
      return rawBuffer + bufferSize;
    }

  public:
    ByteBuffer() : rawBuffer(NULL), bufferSize(0) {}

    static int initFromFile(const char *path, UniquePtr<ByteBuffer> &result);
    int getByte(size_t offset, uint8_t &byte);
    int getBytes(size_t offset, size_t size, uint8_t *bytes);
    inline uint64_t size() {return bufferSize;}
};

}  // namespace dviview

#endif  // __INCLUDE_ANCILLARY_BYTEBUFFER_H__
