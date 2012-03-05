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