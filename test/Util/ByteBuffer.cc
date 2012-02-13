#include <Util/ByteBuffer.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(ByteBufferTest, EmptyFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::init_from_file("ByteBuffer/Empty", buf), (Diag*)NULL);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 0u);
}

TEST(ByteBufferTest, SingleFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::init_from_file("ByteBuffer/Single", buf), (Diag*)NULL);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 1u);
  ASSERT_EQ(buf->get(0), 'Q');
}

TEST(ByteBufferTest, HelloFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_EQ(ByteBuffer::init_from_file("ByteBuffer/Hello", buf), (Diag*)NULL);
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 5u);
  uint8_t bytes[5] = {0u}; // partial initializer initializes all members to 0.
  ASSERT_EQ(buf->get(0), 'H');
  ASSERT_EQ(buf->get(4), 'o');
  buf->get_bytes(0, 5, bytes);
  ASSERT_EQ(bytes[0], 'H');
  ASSERT_EQ(bytes[1], 'e');
  ASSERT_EQ(bytes[2], 'l');
  ASSERT_EQ(bytes[3], 'l');
  ASSERT_EQ(bytes[4], 'o');
}

TEST(ByteBufferTest, NonExistentFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_NE(ByteBuffer::init_from_file("ByteBuffer/NonExistent", buf), (Diag*)NULL);
  ASSERT_FALSE(buf);
}

