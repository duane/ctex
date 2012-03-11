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
  SPLIT_TOP_SKIP_CODE,
  SPLIT_MAX_DEPTH_CODE,
  VSIZE_CODE,
  HSIZE_CODE,
  BOX_BEGIN_CODE,
  MAX_INTERNAL_CODE = BOX_BEGIN_CODE+256,
};

typedef RegisterFile<MAX_INTERNAL_CODE> Memory;

}

#endif  // _INCLUDE_STATE_CODES_H__