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