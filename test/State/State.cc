#include <State/State.h>
#include "gtest/gtest.h"

using namespace tex;

TEST(StateTest, EmptyState) {
  State state;
  
  // Should have an empty table.
  ASSERT_EQ(0u, state.cs_entries());
  
  // Should have catcodes set correctly for characters.
  ASSERT_EQ((uint8_t)CC_IGNORE, state.catcode(0));
  ASSERT_EQ((uint8_t)CC_CAR_RET, state.catcode('\n'));
  ASSERT_EQ((uint8_t)CC_CAR_RET, state.catcode('\r'));
  ASSERT_EQ((uint8_t)CC_SPACER, state.catcode(' '));
  ASSERT_EQ((uint8_t)CC_COMMENT, state.catcode('%'));
  ASSERT_EQ((uint8_t)CC_INVALID, state.catcode(127));
}

TEST(StateTest, Set) {
  State state;
  ASSERT_EQ(0u, state.cs_entries());
  CommandSequence cs = {UString("hello"), CC_CS_STRING};
  state.set(cs);
  ASSERT_EQ(1u, state.cs_entries());
}

TEST(StateTest, SetGet) {
  State state;
  ASSERT_EQ(0u, state.cs_entries());
  UString str = UString("hello");
  CommandSequence cs = {str, CC_CS_STRING};
  state.set(cs);
  ASSERT_EQ(1u, state.cs_entries());
  CommandSequence *p_cs = state.get(str);
  ASSERT_NE((CommandSequence*)NULL, p_cs);
  ASSERT_TRUE(p_cs->string == str);
  ASSERT_EQ(cs.cmd, p_cs->cmd);
}

