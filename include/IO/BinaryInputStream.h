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

#ifndef __INCLUDE_IO_BINARYINPUTSTREAM_H__
#define __INCLUDE_IO_BINARYINPUTSTREAM_H__

#include <Diag/Diag.h>
#include <IO/Endian.h>
#include <Util/ByteBuffer.h>

namespace tex {

class BinaryInputStream {
private:
  // Disallow copy/assign.
  BinaryInputStream(const BinaryInputStream &);
  BinaryInputStream &operator=(const BinaryInputStream &);

  // Default constructor.
  BinaryInputStream(void) : buf(), curr_offset(0), curr_endian(ENDIAN_HOST) {}


private:
// Internal state.

  UniquePtr<ByteBuffer> buf;
  uint64_t curr_offset;

  unsigned curr_endian;
public:
  /**
   *  Initializes a new BinaryInputStream.
   *  Endian is set to ENDIAN_HOST by default, and the stream is set at offset 0.
   *  @param path Where to find the file.
   *  @param result Where the resulting BinaryInputStream is stored.
   *  @throws Diag* on failure.
   */
  static void init_from_file(const char *path, UniquePtr<BinaryInputStream> &result);
  
  /**
   *  Sets the current endian of the file.
   *  @param endian The new endian to be set.
   */
  void set_endian(signed endian) {
    curr_endian = endian;
  }
   
  /**
   *  Fetches the current offset of the file.
   *  @return the current offset.
   */
  uint64_t offset(void) const {
    return curr_offset;
  }
  
  /** 
   *  Fetches the size of the file.
   *  @return the size of the file.
   */
  uint64_t size(void) const {
    return buf->size();
  }
  
  /** Resets stream to offset of 0. */
  void reset(void) {
    curr_offset = 0;
  }
  
  /**
   *  Seeks to a given offset.
   *  @param offset The new offset to seek to.
   */
  void seek(uint64_t offset) {
    assert(offset < buf->size() && "Attempted to seek past the end of the file.");
    curr_offset = offset;
  }
  
  /**
   *  Reads a byte from the stream.
   *  Increments offset by one.
   *  @param result Where the resulting byte is stored.
   *  @return 0 on success, -1 on EOF.
   */
  int read_uint8(uint8_t &result);
  
  /**
   *  Reads two bytes from the stream.
   *  Increments offset by two bytes.
   *  Byte swaps it from current stream endian to host endian.
   *  @param result Where the resulting byte is stored.
   *  @return 0 on success, -1 on EOF.
   */
  int read_uint16(uint16_t &result);
  
  /**
   *  Reads four bytes from the stream.
   *  Increments offset by four bytes.
   *  Byte swaps it from current stream endian to host endian.
   *  @param result Where the resulting byte is stored.
   *  @return 0 on success, -1 on EOF.
   */
  int read_uint32(uint32_t &result);
  
  /**
   *  Reads eight bytes from the stream.
   *  Increments offset by eight bytes.
   *  Byte swaps it from current stream endian to host endian.
   *  @param result Where the resulting byte is stored.
   *  @return 0 on success, -1 on EOF.
   */
  int read_uint64(uint64_t &result);
  
  /**
   *  Copies bytes from stream to destination.
   *  @param dest The destination memory.
   *  @param size How many bytes to read.
   *  @return 0 on success, -1 on EOF.
   */
   int read_bytes(uint8_t *dest, uint32_t size);
   
  
};

}  // namespace tex

#endif  // __INCLUDE_IO_BINARYINPUTSTREAM_H__