#include <IO/BinaryInputStream.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(BinaryInputStreamTest, NonExistent) {
  UniquePtr<BinaryInputStream> stream;
  Diag *diag = BinaryInputStream::init_from_file("BinaryInputStream/NonExistent", stream);
  ASSERT_NE((Diag*)NULL, diag);
  ASSERT_FALSE(stream);
}

TEST(BinaryInputStreamTest, Empty) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Empty", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(0u, stream->size());
  uint8_t byte;
  uint16_t byte16;
  uint32_t byte32;
  uint64_t byte64;
  ASSERT_EQ(-1, stream->read_uint8(byte));
  ASSERT_EQ(-1, stream->read_uint16(byte16));
  ASSERT_EQ(-1, stream->read_uint32(byte32));
  ASSERT_EQ(-1, stream->read_uint64(byte64));
}

TEST(BinaryInputStreamTest, Single) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Byte", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(1u, stream->size());
  uint8_t byte = 0x0;
  ASSERT_EQ(0, stream->read_uint8(byte));
  ASSERT_EQ(0x51u, byte);
  ASSERT_EQ(1u, stream->offset());
  stream->reset();
  byte = 0;
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(0, stream->read_uint8(byte));
  ASSERT_EQ(0x51, byte);
}

#define EXPECT_READ(expected, var, stream, method)\
  ASSERT_EQ(0, stream->method(var));\
  ASSERT_EQ(expected, var);

TEST(BinaryInputStreamTest, Byte16) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Byte16", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(2u, stream->size());
  
  // First test a single double 
  uint8_t byte = 0x0;
  EXPECT_READ(0x51u, byte, stream, read_uint8);
  EXPECT_READ(0x69u, byte, stream, read_uint8);
  ASSERT_EQ(-1, stream->read_uint8(byte));

  uint16_t byte16;

  // Now do a little-endian 16-byte read.
  stream->reset();
  ASSERT_EQ(0u, stream->offset());
  stream->set_endian(ENDIAN_LITTLE);
  EXPECT_READ(0x6951u, byte16, stream, read_uint16);
  ASSERT_EQ(2u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint16(byte16));
  
  // and a big-endian 16-byte read
  stream->reset();
  ASSERT_EQ(0u, stream->offset());
  stream->set_endian(ENDIAN_BIG);
  EXPECT_READ(0x5169u, byte16, stream, read_uint16);
  ASSERT_EQ(2u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint16(byte16));
}

TEST(BinaryInputStreamTest, Byte24) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Byte24", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(3u, stream->size());
  
  // three-byte read first.
  uint8_t byte = 0x0;
  EXPECT_READ(0x51u, byte, stream, read_uint8);
  EXPECT_READ(0x69u, byte, stream, read_uint8);
  EXPECT_READ(0x32u, byte, stream, read_uint8);
  ASSERT_EQ(3u, stream->offset());
  
  // Now little-endian read.
  stream->reset();
  stream->set_endian(ENDIAN_LITTLE);
  ASSERT_EQ(0u, stream->offset());
  uint16_t byte16 = 0x0;
  EXPECT_READ(0x51u, byte, stream, read_uint8);
  EXPECT_READ(0x3269u, byte16, stream, read_uint16);
  ASSERT_EQ(3u, stream->offset());
  
  // Now big-endian.
  stream->reset();
  stream->set_endian(ENDIAN_BIG);
  ASSERT_EQ(0u, stream->offset());
  EXPECT_READ(0x51u, byte, stream, read_uint8);
  EXPECT_READ(0x6932u, byte16, stream, read_uint16);
  ASSERT_EQ(3u, stream->offset());
  
  // Now check that 32-bit reads don't work.
  stream->reset();
  ASSERT_EQ(0u, stream->offset());
  uint32_t byte32;
  ASSERT_EQ(-1, stream->read_uint32(byte32));
}

TEST(BinaryInputStreamTest, Byte32) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Byte32", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(4u, stream->size());
  
  // byte-by-byte read first.
  uint8_t byte;
  EXPECT_READ(0xDEu, byte, stream, read_uint8);
  EXPECT_READ(0xADu, byte, stream, read_uint8);
  EXPECT_READ(0xBEu, byte, stream, read_uint8);
  EXPECT_READ(0xEFu, byte, stream, read_uint8);
  ASSERT_EQ(4u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint8(byte));
  
  // little-endian reads.
  stream->reset();
  stream->set_endian(ENDIAN_LITTLE);
  ASSERT_EQ(0u, stream->offset());
  uint16_t byte16;
  uint32_t byte32;
  EXPECT_READ(0xADDEu, byte16, stream, read_uint16);
  EXPECT_READ(0xEFBEu, byte16, stream, read_uint16);
  ASSERT_EQ(4u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint16(byte16));
  
  stream->reset();
  EXPECT_READ(0xEFBEADDEu, byte32, stream, read_uint32);
  ASSERT_EQ(4u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint32(byte32));
  
  // big-endian
  stream->reset();
  stream->set_endian(ENDIAN_BIG);
  ASSERT_EQ(0u, stream->offset());
  EXPECT_READ(0xDEADu, byte16, stream, read_uint16);
  EXPECT_READ(0xBEEFu, byte16, stream, read_uint16);
  ASSERT_EQ(4u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint16(byte16));
  
  stream->reset();
  ASSERT_EQ(0u, stream->offset());
  EXPECT_READ(0xDEADBEEFu, byte32, stream, read_uint32);
  ASSERT_EQ(4u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint32(byte32));
}

TEST(BinaryInputStreamTest, Byte64) {
  UniquePtr<BinaryInputStream> stream;
  ASSERT_EQ(NULL, BinaryInputStream::init_from_file("BinaryInputStream/Byte64", stream));
  ASSERT_TRUE(stream);
  ASSERT_EQ(0u, stream->offset());
  ASSERT_EQ(8u, stream->size());
  
  // No need to check all reads again. Just check 64-bit.
  // little endian first.
  stream->set_endian(ENDIAN_LITTLE);
  uint64_t byte64;
  EXPECT_READ(0xEFBEBEBAFECAADDEu, byte64, stream, read_uint64);
  ASSERT_EQ(8u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint64(byte64));
  
  // big endian mode.
  stream->reset();
  stream->set_endian(ENDIAN_BIG);
  ASSERT_EQ(0u, stream->offset());
  EXPECT_READ(0xDEADCAFEBABEBEEFu, byte64, stream, read_uint64);
  ASSERT_EQ(8u, stream->offset());
  ASSERT_EQ(-1, stream->read_uint64(byte64));
}