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

#include <IO/Path.h>
#include "gtest/gtest.h"

#include <iostream>

using namespace tex;

TEST(PathTest, EmptyTest) {
  Path path;
  ASSERT_TRUE(path.area() == "");
  ASSERT_TRUE(path.file() == "");
  ASSERT_TRUE(path.ext() == "");
  ASSERT_TRUE(path.full_path() == "");
}

TEST(PathTest, SimpleTest) {
  Path path("hello");
  ASSERT_TRUE(path.area() == "");
  ASSERT_TRUE(path.file() == "hello");
  ASSERT_TRUE(path.ext() == "");
  ASSERT_TRUE(path.full_path() == "hello");
}

TEST(PathTest, SimpleExtTest) {
  Path path("hello.tex");
  ASSERT_TRUE(path.area() == "");
  ASSERT_TRUE(path.file() == "hello");
  ASSERT_TRUE(path.ext() == "tex");
  ASSERT_TRUE(path.full_path() == "hello.tex");
}

TEST(PathTest, SimpleDirTest) {
  Path path("dir/hello");
  ASSERT_TRUE(path.area() == "dir/");
  ASSERT_TRUE(path.file() == "hello");
  ASSERT_TRUE(path.ext() == "");
  ASSERT_TRUE(path.full_path() == "dir/hello");
}

TEST(PathTest, DirExtTest) {
  Path path("dir/hello.tex");
  ASSERT_TRUE(path.area() == "dir/");
  ASSERT_TRUE(path.file() == "hello");
  ASSERT_TRUE(path.ext() == "tex");
  ASSERT_TRUE(path.full_path() == "dir/hello.tex");
}

TEST(PathTest, MultipleDotExtTest) {
  Path path("Dir.temp/hello.first.tex");
  ASSERT_TRUE(path.area() == "Dir.temp/");
  ASSERT_TRUE(path.file() == "hello.first");
  ASSERT_TRUE(path.ext() == "tex");
  ASSERT_TRUE(path.full_path() == "Dir.temp/hello.first.tex");
}

TEST(PathTest, DotFile) {
  Path path("~/.bash_profile");
  ASSERT_TRUE(path.area() == "~/");
  ASSERT_TRUE(path.file() == ".bash_profile");
  ASSERT_TRUE(path.ext() == "");
  ASSERT_TRUE(path.full_path() == "~/.bash_profile");
}