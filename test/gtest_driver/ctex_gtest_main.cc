#include <cstdio>

#include "gtest/gtest.h"

class CTeXTestPrinter : public ::testing::EmptyTestEventListener {

  virtual void OnTestProgramStart(const ::testing::UnitTest &unit_info) {
    printf("DRIVER %d cases, %d tests.\n",
           unit_info.total_test_case_count(), unit_info.total_test_count());
  }

  virtual void OnTestCaseStart(const ::testing::TestCase &case_info) {
    printf("CASE %s, %d tests.\n", case_info.name(),
           case_info.total_test_count());
  }

    // Called before a test starts.
  virtual void OnTestStart(const ::testing::TestInfo& test_info) {
    printf("START %s\n", test_info.name());
  }

  // Called after a failed assertion or a SUCCEED() invocation.
  virtual void OnTestPartResult(
      const ::testing::TestPartResult& test_part_result) {
    if (test_part_result.failed())
        printf("FAILED in %s:%d\n%s\n",
           test_part_result.file_name(),
           test_part_result.line_number(),
           test_part_result.summary());
    else
      printf("SUCCESS\n");
  }

  // Called after a test ends.
  virtual void OnTestEnd(const ::testing::TestInfo& test_info) {
    printf("SUCCESS\n");
  }
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  // Gets hold of the event listener list.
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
  // Adds a listener to the end.  Google Test takes the ownership.
  delete listeners.Release(listeners.default_result_printer());
  listeners.Append(new CTeXTestPrinter);
  return RUN_ALL_TESTS();
}