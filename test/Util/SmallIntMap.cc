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

#include <Util/SmallIntMap.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(SmallIntMapTest, EmptyTest) {
  SmallIntMap<uint32_t, 8> empty;
  ASSERT_EQ(0u, empty.entries());
}

TEST(SmallIntMapTest, SetGet) {
  SmallIntMap<uint32_t, 8> map;
  for (uint32_t i = 0; i < 64; i++) {
    map.set(i, i);
  }
  ASSERT_EQ(64u, map.entries());
  for (uint32_t k = 0; k < 64; k++) {
    const uint32_t *v = map.get(k);
    ASSERT_TRUE(v != NULL);
    ASSERT_EQ(k, *v);
  }
  ASSERT_EQ(NULL, map.get(64));
}