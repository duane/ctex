#include <Unicode/Codecs/ASCII.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(ASCIITest, SemiExhaustiveEncode) {
  ASCIICodec codec;
  EncodedCharacter ec;
  for (unichar uchar = 0; uchar < 128; uchar++) {
    ec = codec.encode(uchar);
    ASSERT_EQ(ec.length, 1u);
    ASSERT_EQ(ec.encoded[0], uchar);
  }
  ec = codec.encode(128);
  ASSERT_EQ(ec.length, 2u);
  ASSERT_EQ(ec.encoded[0], 0xFF);
  ASSERT_EQ(ec.encoded[1], 0XF4);
}

TEST(ASCIITEST, SemiExhaustiveDecode) {
  ASCIICodec codec;
  DecodedCharacter dc;
  for (uint8_t ascii = 0; ascii < 128; ascii++) {
    dc = codec.decode(&ascii, 0, 1);
    ASSERT_EQ(dc.length, 1);
    ASSERT_EQ(dc.uchar, ascii);
  }
  uint8_t bad = 128;
  dc = codec.decode(&bad, 0, 1);
  ASSERT_EQ(dc.length, 1);
  ASSERT_EQ(dc.uchar, 0xFFF4u);
}