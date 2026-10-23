// File basic_functions_test.cc: tests for the basic functions (zero,
// successor, projection) and for the arity check of Function::Evaluate.

#include <gtest/gtest.h>

#include <limits>

#include "exceptions/exceptions.h"
#include "functions/projection_function.h"
#include "functions/successor_function.h"
#include "functions/zero_function.h"

TEST(ZeroFunctionTest, ReturnsZeroWhateverTheArguments) {
  CallCounter counter;
  EXPECT_EQ(ZeroFunction().Evaluate({7}, counter), 0u);
  EXPECT_EQ(ZeroFunction(3).Evaluate({1, 2, 3}, counter), 0u);
}

TEST(ZeroFunctionTest, ArityAndDescription) {
  EXPECT_EQ(ZeroFunction().Arity(), 1u);
  EXPECT_EQ(ZeroFunction(3).Arity(), 3u);
  EXPECT_EQ(ZeroFunction(3).Describe(), "Z^3");
}

TEST(ZeroFunctionTest, ArityZeroIsInvalid) {
  EXPECT_THROW(ZeroFunction(0), InvalidFunctionException);
}

TEST(SuccessorFunctionTest, AddsOne) {
  CallCounter counter;
  SuccessorFunction successor;
  EXPECT_EQ(successor.Evaluate({0}, counter), 1u);
  EXPECT_EQ(successor.Evaluate({41}, counter), 42u);
  EXPECT_EQ(successor.Arity(), 1u);
  EXPECT_EQ(successor.Describe(), "S");
}

TEST(SuccessorFunctionTest, LargestRepresentableValueStillWorks) {
  CallCounter counter;
  Natural max = std::numeric_limits<Natural>::max();
  EXPECT_EQ(SuccessorFunction().Evaluate({max - 1}, counter), max);
}

TEST(SuccessorFunctionTest, OverflowIsDetected) {
  CallCounter counter;
  Natural max = std::numeric_limits<Natural>::max();
  EXPECT_THROW(SuccessorFunction().Evaluate({max}, counter),
               NaturalOverflowException);
}

TEST(ProjectionFunctionTest, ReturnsTheChosenArgument) {
  CallCounter counter;
  EXPECT_EQ(ProjectionFunction(1, 3).Evaluate({10, 20, 30}, counter), 10u);
  EXPECT_EQ(ProjectionFunction(2, 3).Evaluate({10, 20, 30}, counter), 20u);
  EXPECT_EQ(ProjectionFunction(3, 3).Evaluate({10, 20, 30}, counter), 30u);
  EXPECT_EQ(ProjectionFunction(1, 1).Evaluate({5}, counter), 5u);
}

TEST(ProjectionFunctionTest, ArityAndDescription) {
  ProjectionFunction projection(2, 3);
  EXPECT_EQ(projection.Arity(), 3u);
  EXPECT_EQ(projection.Describe(), "P^3_2");
}

TEST(ProjectionFunctionTest, IndexOutOfRangeIsInvalid) {
  EXPECT_THROW(ProjectionFunction(0, 3), InvalidFunctionException);
  EXPECT_THROW(ProjectionFunction(4, 3), InvalidFunctionException);
  EXPECT_THROW(ProjectionFunction(1, 0), InvalidFunctionException);
}

TEST(FunctionTest, EveryEvaluationCountsOneCall) {
  CallCounter counter;
  SuccessorFunction successor;
  successor.Evaluate({1}, counter);
  successor.Evaluate({2}, counter);
  EXPECT_EQ(counter.Count(), 2u);
}

TEST(FunctionTest, WrongNumberOfArgumentsThrowsAndIsNotCounted) {
  CallCounter counter;
  EXPECT_THROW(SuccessorFunction().Evaluate({1, 2}, counter),
               InvalidArityException);
  EXPECT_THROW(ProjectionFunction(1, 2).Evaluate({}, counter),
               InvalidArityException);
  EXPECT_EQ(counter.Count(), 0u);
}
