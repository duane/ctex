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

#include <IO/CodecInputStream.h>
#include <Unicode/Codecs/ASCII.h>
#include <Unicode/Codecs/UTF8.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(CodecInputStreamTest, EmptyFile) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_NO_THROW(CodecInputStream::init_from_file("CodecInputStream/Empty", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  unichar uc;
  ASSERT_EQ(-1, input_stream->peek_char(uc));
  ASSERT_EQ(-1, input_stream->consume_char(uc));
}

TEST(CodecInputStreamTest, SingleTest) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_NO_THROW(CodecInputStream::init_from_file("CodecInputStream/Single", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  unichar uc;
  ASSERT_EQ(0, input_stream->peek_char(uc));
  ASSERT_EQ(0x03BBu, uc);
  ASSERT_EQ(0, input_stream->peek_char(uc));
  ASSERT_EQ(0x03BBu, uc);
  ASSERT_EQ(0, input_stream->consume_char(uc));
  ASSERT_EQ(0x03BBu, uc);
  ASSERT_EQ(-1, input_stream->peek_char(uc));
  ASSERT_EQ(-1, input_stream->peek_char(uc));
  ASSERT_EQ(-1, input_stream->consume_char(uc));
}

TEST(CodecInputStreamTest, DoubleConsumeTest) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_NO_THROW(CodecInputStream::init_from_file("CodecInputStream/Double", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  unichar uc;
  ASSERT_EQ(0, input_stream->consume_char(uc));
  ASSERT_EQ(0x03B5u, uc);
  ASSERT_EQ(0, input_stream->consume_char(uc));
  ASSERT_EQ(0x03C5u, uc);
  ASSERT_EQ(-1, input_stream->consume_char(uc));
}

TEST(CodecInputStreamTest, NonExistantFile) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_THROW(CodecInputStream::init_from_file("CodecInputStream/NonExistant", &codec, input_stream), Diag*);
  ASSERT_FALSE(input_stream);
}