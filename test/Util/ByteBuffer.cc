#include <Util/ByteBuffer.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(ByteBufferTest, EmptyInit) {
  ByteBuffer *buf = new ByteBuffer();
  ASSERT_FALSE(buf == NULL);
  ASSERT_EQ(buf->size(), 0u);
}

TEST(ByteBufferTest, EmptyFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::initFromFile("ByteBuffer/Empty", buf), 0);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 0u);
}

TEST(ByteBufferTest, SingleFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::initFromFile("ByteBuffer/Single", buf), 0);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 1u);
  uint8_t byte;
  ASSERT_EQ(buf->getByte(0, byte), 0);
  ASSERT_EQ(byte, 'Q');
}

TEST(ByteBufferTest, HelloFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::initFromFile("ByteBuffer/Hello", buf), 0);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 5u);
  uint8_t bytes[5] = {0u}; // partial initializer initializes all members to 0.
  ASSERT_EQ(buf->getByte(0, bytes[0]), 0);
  ASSERT_EQ(bytes[0], 'H');
  ASSERT_EQ(buf->getByte(4, bytes[0]), 0);
  ASSERT_EQ(bytes[0], 'o');
  ASSERT_EQ(buf->getBytes(0, 5, bytes), 0);
  ASSERT_EQ(bytes[0], 'H');
  ASSERT_EQ(bytes[1], 'e');
  ASSERT_EQ(bytes[2], 'l');
  ASSERT_EQ(bytes[3], 'l');
  ASSERT_EQ(bytes[4], 'o');
}

TEST(ByteBufferTest, Bounds) {
  // No need to check for negatives; all offset and size parameters are unsigned.
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::initFromFile("ByteBuffer/Hello", buf), 0);
  ASSERT_EQ(buf->size(), 5u);
  uint8_t byte = 0;
  ASSERT_FALSE(buf->getByte(5, byte) == 0);
  uint8_t bytes[6] = {0u};
  ASSERT_FALSE(buf->getBytes(0, 6, bytes) == 0);
  ASSERT_FALSE(buf->getBytes(1, 5, bytes) == 0);
}

TEST(ByteBufferTest, NonExistentFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_FALSE(ByteBuffer::initFromFile("ByteBuffer/NonExistent", buf) == 0);
  ASSERT_FALSE(buf);
}

