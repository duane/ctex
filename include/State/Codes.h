#ifndef __INCLUDE_STATE_CODES_H__
#define __INCLUDE_STATE_CODES_H__

#include <State/RegisterFile.h>

namespace tex {

enum {
  PRETOLERANCE_CODE,
  LEFT_SKIP_CODE,
  RIGHT_SKIP_CODE,
  PARINDENT_CODE,
  BASELINE_SKIP_CODE,
  VSIZE_CODE,
  HSIZE_CODE,
  MAX_INTERNAL_CODE,
};

typedef RegisterFile<MAX_INTERNAL_CODE> Memory;

}

#endif  // _INCLUDE_STATE_CODES_H__