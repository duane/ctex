#include <Parse/TokenInputStream.h>
#include <Unicode/Codecs/UTF8.h>
#include "gtest/gtest.h"

#include <cstdio>

using namespace tex;

TEST(TokenInputStreamTest, NonExistentFile) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  Diag *diag = TokenInputStream::init_from_file("TokenInputStream/NonExistent", &codec, input_stream);
  ASSERT_NE((Diag*)NULL, diag);
  ASSERT_FALSE(input_stream);
}

#define EXPECT_CMD(stream, tok, state, cc)\
  ASSERT_EQ(NULL, stream->consume_token(state, tok));\
  ASSERT_EQ((CommandCode)cc, tok.cmd);

TEST(TokenInputStreamTest, Empty) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  State state;
  ASSERT_EQ(NULL, TokenInputStream::init_from_file("TokenInputStream/Empty", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  ASSERT_EQ(NULL, input_stream->consume_token(state, token));
  ASSERT_EQ((CommandCode)CC_EOF, token.cmd);
}

TEST(TokenInputStreamTest, Spaces) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  State state;
  ASSERT_EQ(NULL, TokenInputStream::init_from_file("TokenInputStream/Spaces", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_EOF);
}

TEST(TokenInputStreamTest, Newlines) {
  UTF8Codec codec;
  UniquePtr<TokenInputStream> input_stream;
  State state;
  ASSERT_EQ(NULL, TokenInputStream::init_from_file("TokenInputStream/Newlines", &codec, input_stream));
  ASSERT_TRUE(input_stream);
  Token token;
  UString par = UString("par");
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_CS_STRING);
  ASSERT_TRUE(token.string->equalq(par));
  EXPECT_CMD(input_stream, token, state, CC_LETTER);
  EXPECT_CMD(input_stream, token, state, CC_SPACER);
  EXPECT_CMD(input_stream, token, state, CC_EOF);
}

