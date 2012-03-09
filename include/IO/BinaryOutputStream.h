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

#ifndef __INCLUDE_BINARYOUTPUTSTREAM_H__
#define __INCLUDE_BINARYOUTPUTSTREAM_H__

#include <cstdio>

#include <Diag/Diag.h>
#include <IO/Endian.h>
#include <Util/UniquePtr.h>

namespace tex {

class BinaryOutputStream {
private:
  BinaryOutputStream(const BinaryOutputStream&);
  const BinaryOutputStream &operator=(const BinaryOutputStream &);
  
  BinaryOutputStream(void) : output(NULL), curr_offset(0), curr_endian(ENDIAN_HOST) {}

private:
  FILE *output;
  
  
  uint64_t curr_offset;
  unsigned curr_endian;

public:
  /**
   *
   */
  static void init_with_file(const char *path, UniquePtr<BinaryOutputStream> &result);
  
  void set_endian(unsigned endian) {
    assert((endian == ENDIAN_BIG || endian == ENDIAN_LITTLE) && "Attempted to use invalid endian.");
    curr_endian = endian;
  }
  
  uint64_t offset(void) const {
    return curr_offset;
  }
  
  Diag *write_uint8(uint8_t byte);
  
  Diag *write_uint16(uint16_t byte16);
  
  Diag *write_uint24(uint32_t byte24);
  
  Diag *write_uint32(uint32_t byte32);
  
  Diag *write_uint64(uint64_t byte64);
  
  Diag *write_bytes(uint8_t *bytes, size_t len);
  
  ~BinaryOutputStream(void);
};

}

#endif  // __INCLUDE_BINARYOUTPUTSTREAM_H__