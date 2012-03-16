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
  ASSERT_EQ(1u, state->eqtb()[FONT_CODE].u64);
}
