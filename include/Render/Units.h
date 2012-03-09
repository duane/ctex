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

#ifndef __INCLUDE_RENDER_UNITS_H__
#define __INCLUDE_RENDER_UNITS_H__

namespace tex {

enum unit_measure {
  UNIT_IN,
  UNIT_PC,
  UNIT_CM,
  UNIT_MM,
  UNIT_BP,
  UNIT_DD,
  UNIT_CC,
  UNIT_SP,
};

struct unit_conversion {
  uint32_t num;
  uint32_t den;
};

const unit_conversion UNIT_CONV[] = {
  {7227,100},
  {12,1},
  {7227,254},
  {7227, 2540},
  {7227, 7200},
  {1238, 1157},
  {14856, 1157},
};

static inline sp scaled_from(int32_t num, unit_measure unit) {
  unit_conversion conv = UNIT_CONV[unit];
  return scaled(num).xn_over_d(conv.num, conv.den);
}

}

#endif  // __INCLUDE_RENDER_UNITS_H__