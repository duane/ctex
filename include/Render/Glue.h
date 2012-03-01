#ifndef __INCLUDE_RENDER_GLUE_H__
#define __INCLUDE_RENDER_GLUE_H__

#include <stdint.h>

#include <Render/sp.h>

namespace tex {

enum glue_order {
  GLUE_NORMAL,
  GLUE_FIL,
  GLUE_FILL,
  GLUE_FILLL
};

struct glue_node {
  sp width, stretch, shrink;
  glue_order stretch_order, shrink_order;
};

}

#endif  // __INCLUDE_RENDER_GLUE_H__