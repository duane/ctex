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

#include <Parse/TokenInputStream.h>
#include <Unicode/Codecs/UTF8.h>
#include "gtest/gtest.h"

#include <cstdio>

using namespace tex;

TEST(TokenInputStreamTest, NonExistentFile) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  ASSERT_THROW(TokenInputStream::init_from_file("TokenInputStream/NonExistent", &codec, input_stream), Diag*);
  ASSERT_FALSE(input_stream);
}

#define EXPECT_CMD(stream, tok, state, cc)\
  ASSERT_NO_THROW({\
    ASSERT_EQ(0, stream->consume_token(state, tok));\
    ASSERT_EQ((CommandCode)cc, tok.cmd);\
  })

TEST(TokenInputStreamTest, Empty) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  UniquePtr<State> state;
  State::init(state);
  ASSERT_NO_THROW(TokenInputStream::init_from_file("TokenInputStream/Empty", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  EXPECT_CMD(input_stream, token, state, CC_STOP);
}

TEST(TokenInputStreamTest, Spaces) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  UniquePtr<State> state;
  State::init(state);
  ASSERT_NO_THROW(TokenInputStream::init_from_file("TokenInputStream/Spaces", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  ASSERT_NO_THROW({
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_STOP);
  });
}

TEST(TokenInputStreamTest, Newlines) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  UniquePtr<State> state;
  State::init(state);
  ASSERT_NO_THROW(TokenInputStream::init_from_file("TokenInputStream/Newlines", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  ASSERT_NO_THROW({
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_PAR_END);
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_STOP);
  });
}

