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

#include <Unicode/UString.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(MutableUStringTest, EmptyInit) {
  MutableUString string;
  ASSERT_EQ(string.get_length(), 0u);
  ASSERT_GT(string.get_allocated(), 0u);
}

TEST(MutableUStringTest, UStringCopy) {
  UString string = UString("Hello");
  MutableUString mut_string = MutableUString(string);
  ASSERT_TRUE(mut_string == string);
  ASSERT_TRUE(mut_string == string);
  ASSERT_EQ(string.get_length(), mut_string.get_length());
  ASSERT_EQ(string.hash(), mut_string.hash());
}

TEST(MutableUStringTest, Add) {
  MutableUString string;
  ASSERT_EQ(string.get_length(), 0u);
  string.add((unichar)'Q');
  ASSERT_EQ(string.get_length(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'Q');
}

TEST(MutableUStringTest, Set) {
  MutableUString string;
  string.add((unichar)'Q');
  ASSERT_EQ(string.get_length(), 1u);
  string.set(0, (unichar)'X');
  ASSERT_EQ(string.get_length(), 1u);
  ASSERT_EQ(string.get(0), (unichar)'X');
}