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

#include <Util/SmallVector.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(SmallVectorTest, EmptyTest) {
  SmallVector<uint32_t, 16> vector;
  ASSERT_EQ(0u, vector.entries());
}

TEST(SmallVectorTest, AppendGet) {
  SmallVector<uint32_t, 16> vector;
  ASSERT_EQ(0u, vector.entries());
  uint32_t val = 42u;
  vector.append(val);
  ASSERT_EQ(1u, vector.entries());
  ASSERT_EQ(val, vector[0]);
}

TEST(SmallVectorTest, ExpandBeyondBoundsTest) {
  SmallVector<uint32_t, 2> vector;
  ASSERT_EQ(0u, vector.entries());
  uint32_t val = 0;
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(2u, vector.entries());
  ASSERT_EQ(0u, vector[0]);
  ASSERT_EQ(1u, vector[1]);
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(4u, vector.entries());
  ASSERT_EQ(0u, vector[0]);
  ASSERT_EQ(1u, vector[1]);
  ASSERT_EQ(2u, vector[2]);
  ASSERT_EQ(3u, vector[3]);
  vector.append(val);
  val++;
  vector.append(val);
  val++;
  ASSERT_EQ(6u, vector.entries());
  ASSERT_EQ(0u, vector[0]);
  ASSERT_EQ(1u, vector[1]);
  ASSERT_EQ(2u, vector[2]);
  ASSERT_EQ(3u, vector[3]);
  ASSERT_EQ(4u, vector[4]);
  ASSERT_EQ(5u, vector[5]);
}