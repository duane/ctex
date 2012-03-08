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

