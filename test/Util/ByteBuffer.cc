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

#include <Util/ByteBuffer.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(ByteBufferTest, EmptyFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_NO_THROW(ByteBuffer::init_from_file("ByteBuffer/Empty", buf));
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 0u);
}

TEST(ByteBufferTest, SingleFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_NO_THROW(ByteBuffer::init_from_file("ByteBuffer/Single", buf));
  ASSERT_TRUE(buf);
  ASSERT_EQ(buf->size(), 1u);
  ASSERT_EQ(buf->get(0), 'Q');
}

TEST(ByteBufferTest, HelloFile) {
  UniquePtr<ByteBuffer> buf;
  ASSERT_NO_THROW(ByteBuffer::init_from_file("ByteBuffer/Hello", buf));
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
  ASSERT_THROW(ByteBuffer::init_from_file("ByteBuffer/NonExistent", buf), Diag*);
  ASSERT_FALSE(buf);
}

