#ifndef __INCLUDE_RENDER_CHAR_H__
#define __INCLUDE_RENDER_CHAR_H__

#include <stdint.h>

namespace tex {

struct char_node {
  uint8_t ch;
  uint8_t font;
};

}

#endif  // __INCLUDE_RENDER_CHAR_H__