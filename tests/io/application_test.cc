// File application_test.cc: end-to-end tests for Application::Run,
// using an ostringstream instead of std::cout.

#include "io/application.h"

#include <gtest/gtest.h>

#include <sstream>

#include "exceptions/exceptions.h"

namespace {

std::string RunWith(Natural x, Natural y, std::uint64_t limit = CallCounter::kDefaultLimit) {
  Options options;
  options.base = x;
  options.exponent = y;
  options.max_calls = limit;
  std::ostringstream out;
  Application(options, out).Run();
  return out.str();
}

}  // namespace

TEST(ApplicationTest, PrintsTheResultAndTheNumberOfCalls) {
  EXPECT_EQ(RunWith(2, 10), "power(2, 10) = 1024\nFunction calls: 8368\n");
}

TEST(ApplicationTest, ZeroToTheZeroIsOne) {
  EXPECT_EQ(RunWith(0, 0), "power(0, 0) = 1\nFunction calls: 4\n");
}

TEST(ApplicationTest, ExponentZeroAndOne) {
  EXPECT_EQ(RunWith(9, 0), "power(9, 0) = 1\nFunction calls: 4\n");
  EXPECT_NE(RunWith(9, 1).find("power(9, 1) = 9\n"), std::string::npos);
}

TEST(ApplicationTest, ZeroBase) {
  EXPECT_NE(RunWith(0, 7).find("power(0, 7) = 0\n"), std::string::npos);
}

TEST(ApplicationTest, ExceedingTheCallLimitThrows) {
  EXPECT_THROW(RunWith(10, 10, 1000), CallLimitExceededException);
}

TEST(ApplicationTest, ARaisedLimitAllowsALongerComputation) {
  EXPECT_THROW(RunWith(2, 10, 8367), CallLimitExceededException);
  EXPECT_NO_THROW(RunWith(2, 10, 8368));
}

TEST(ApplicationTest, DefaultOptionsUseTheDefaultLimit) {
  Options options;
  EXPECT_EQ(options.max_calls, CallCounter::kDefaultLimit);
}
