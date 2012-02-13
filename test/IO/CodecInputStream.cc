#include <IO/CodecInputStream.h>
#include <Unicode/Codecs/ASCII.h>
#include <Unicode/Codecs/UTF8.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(CodecInputStreamTest, EmptyFile) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_EQ(NULL, CodecInputStream::init_from_file("CodecInputStream/Empty", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  unichar uc;
  ASSERT_EQ(-1, input_stream->peek_char(uc));
  ASSERT_EQ(-1, input_stream->consume_char(uc));
}

TEST(CodecInputStreamTest, SingleTest) {
  UTF8Codec codec;
  UniquePtr<CodecInputStream> input_stream;
  ASSERT_EQ(NULL, CodecInputStream::init_from_file("CodecInputStream/Single", &codec, input_stream));
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
  ASSERT_EQ(NULL, CodecInputStream::init_from_file("CodecInputStream/Double", &codec, input_stream));
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
  Diag *diag = CodecInputStream::init_from_file("CodecInputStream/NonExistant", &codec, input_stream);
  ASSERT_NE((Diag*)NULL, diag);
  ASSERT_FALSE(input_stream);
  ASSERT_EQ((int)DIAG_FILE_OPEN_ERR, diag->errno());
}