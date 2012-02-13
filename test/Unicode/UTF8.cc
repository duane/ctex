#include <Unicode/Codecs/UTF8.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(UTF8Test, ASCIIEncode) {
  UTF8Codec codec;
  EncodedCharacter ec;
  for (unichar uchar = 0; uchar < 128; uchar++) {
    ec = codec.encode(uchar);
    ASSERT_EQ(ec.length, 1u);
    ASSERT_EQ(ec.encoded[0], uchar);
  }
}

TEST(UTF8Test, ASCIIDecode) {
  UTF8Codec codec;
  DecodedCharacter dc;
  for (uint8_t ascii = 0; ascii < 128; ascii++) {
    dc = codec.decode(&ascii, 0, 1);
    ASSERT_EQ(dc.length, 1);
    ASSERT_EQ(dc.uchar, ascii);
  }
}

TEST(UTF8Test, UTF8BadDecode) {
  UTF8Codec codec;
  uint8_t bad1[] = {0xC2};
  DecodedCharacter dc = codec.decode(bad1, 0, 1);
  ASSERT_EQ(dc.length, 2);
  ASSERT_EQ(dc.uchar, 0xFFF4u);
  uint8_t invalid_codepoints[] = {0xC0, 0xC1, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFD};
  uint8_t bytes[4] = {0x0, 0x80, 0x80, 0x80};
  for (unsigned i = 0; i < sizeof(invalid_codepoints); i++) {
    bytes[0] = invalid_codepoints[i];
    dc = codec.decode(bytes, 0, 4);
    ASSERT_EQ(dc.length, 2);
    ASSERT_EQ(dc.uchar, 0xFFF4u);
  }
}

TEST(UTF8Test, UTF8LimitEncode) {
  // Such a test is easy. Simply check the limit of unicode, 0x10FFFF
  UTF8Codec codec;
  unichar limit = 0x10FFFF;
  EncodedCharacter ec = codec.encode(limit);
  ASSERT_EQ(ec.length, 4);
  ASSERT_EQ(ec.encoded[0], 0xF4u);
  ASSERT_EQ(ec.encoded[1], 0x8Fu);
  ASSERT_EQ(ec.encoded[2], 0xBFu);
  ASSERT_EQ(ec.encoded[3], 0xBFu);
  ec = codec.encode(limit + 1);
  ASSERT_EQ(ec.length, 3);
  ASSERT_EQ(ec.encoded[0], 0xEFu);
  ASSERT_EQ(ec.encoded[1], 0xBFu);
  ASSERT_EQ(ec.encoded[2], 0xB4u);
}