// File call_counter_test.cc: tests for CallCounter.

#include "functions/call_counter.h"

#include <gtest/gtest.h>

#include "exceptions/exceptions.h"

TEST(CallCounterTest, StartsAtZeroWithTheDefaultLimit) {
  CallCounter counter;
  EXPECT_EQ(counter.Count(), 0u);
  EXPECT_EQ(counter.Limit(), CallCounter::kDefaultLimit);
}

TEST(CallCounterTest, IncrementCounts) {
  CallCounter counter;
  counter.Increment();
  counter.Increment();
  EXPECT_EQ(counter.Count(), 2u);
}

TEST(CallCounterTest, ReachingTheLimitIsAllowed) {
  CallCounter counter(3);
  counter.Increment();
  counter.Increment();
  EXPECT_NO_THROW(counter.Increment());
  EXPECT_EQ(counter.Count(), 3u);
}

TEST(CallCounterTest, GoingOverTheLimitThrows) {
  CallCounter counter(2);
  counter.Increment();
  counter.Increment();
  EXPECT_THROW(counter.Increment(), CallLimitExceededException);
  EXPECT_EQ(counter.Count(), 2u);
}
