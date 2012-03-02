#ifndef __INCLUDE_RENDER_GLUE_H__
#define __INCLUDE_RENDER_GLUE_H__

#include <stdint.h>

#include <Render/sp.h>

namespace tex {

typedef float glue_ratio;

enum glue_order {
  GLUE_NORMAL = 0,
  GLUE_FIL,
  GLUE_FILL,
  GLUE_FILLL
};

enum glue_sign {
  SIGN_NORMAL = 0,
  SIGN_STRETCH,
  SIGN_SHRINK,
};

struct glue_node {
  sp width, stretch, shrink;
  glue_order stretch_order, shrink_order;
};

}

#endif  // __INCLUDE_RENDER_GLUE_H__