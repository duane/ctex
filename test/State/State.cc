#include <State/State.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(StateTest, EmptyState) {
  UniquePtr<State> state;
  State::init(state);

  // Should have catcodes set correctly for characters.
  ASSERT_EQ((uint8_t)CC_IGNORE, state->catcode(0));
  ASSERT_EQ((uint8_t)CC_CAR_RET, state->catcode('\n'));
  ASSERT_EQ((uint8_t)CC_CAR_RET, state->catcode('\r'));
  ASSERT_EQ((uint8_t)CC_SPACER, state->catcode(' '));
  ASSERT_EQ((uint8_t)CC_COMMENT, state->catcode('%'));
  ASSERT_EQ((uint8_t)CC_INVALID, state->catcode(127));
}

/*TEST(StateTest, Set) {
  UniquePtr<State> state;
  State::init(state);
}

TEST(StateTest, SetGet) {
  UniquePtr<State> state;
  State::init(state);
  ASSERT_TRUE(state);
  ASSERT_EQ(0u, state->cs_entries());
  UString str = UString("hello");
  state->primitive(str, 42, (word){83});
  ASSERT_EQ(1u, state->cs_entries());
  CommandSequence *p_cs = state->get(str);
  ASSERT_NE((CommandSequence*)NULL, p_cs);
  ASSERT_TRUE(str == p_cs->string);
  ASSERT_EQ(42u, p_cs->cmd);
  ASSERT_TRUE((word){83} == p_cs->operand);
}*/

