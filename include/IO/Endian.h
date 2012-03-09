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

#ifndef __INCLUDE_IO_ENDIAN_H__
#define __INCLUDE_IO_ENDIAN_H__

#include <stdint.h>

namespace tex {

enum {
  ENDIAN_LITTLE = 0,
  ENDIAN_BIG = 1,
};

static const union {
  uint8_t bytes_val[2];
  uint16_t int_val;
} HOST_ENDIAN_VAL = {{0x0, 0x1}};

static const unsigned ENDIAN_HOST = HOST_ENDIAN_VAL.int_val & 0x1;

static inline void swap_bytes_16(uint16_t &bytes) {
  bytes = (bytes << 8) | (bytes >> 8);
}

static inline void swap_bytes_32(uint32_t &bytes) {
  bytes = ((bytes << 24) | ((bytes  & 0xFF00) << 8) | ((bytes & 0xFF0000) >> 8) | (bytes >> 24));
}

static inline void swap_bytes_64(uint64_t &bytes) {
  bytes = (bytes << 56)
        | ((bytes & 0xFF00ULL) << 40)
        | ((bytes & 0xFF0000ULL)  << 24)
        | ((bytes & 0xFF000000ULL) << 8)

        | ((bytes & 0xFF00000000ULL) >> 8)
        | ((bytes & 0xFF0000000000ULL) >> 24)
        | ((bytes & 0xFF000000000000ULL) >> 40)
        | (bytes >> 56);
}

}

#endif  // __INCLUDE_IO_ENDIAN_H__