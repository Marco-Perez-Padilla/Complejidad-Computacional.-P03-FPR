// File operators_test.cc: tests for the composition and primitive
// recursion operators, with small functions built by hand.

#include <gtest/gtest.h>

#include <limits>

#include "exceptions/exceptions.h"
#include "functions/composition_function.h"
#include "functions/function_library.h"
#include "functions/primitive_recursion_function.h"

// ---- Composition ----

TEST(CompositionFunctionTest, AppliesTheOuterFunctionToTheInnerResults) {
  // Cn[S; P^2_2](x, y) = y + 1
  FunctionPtr f = FunctionLibrary::Composition(FunctionLibrary::Successor(),
                                               {FunctionLibrary::Projection(2, 2)});
  CallCounter counter;
  EXPECT_EQ(f->Evaluate({10, 4}, counter), 5u);
  EXPECT_EQ(f->Arity(), 2u);
}

TEST(CompositionFunctionTest, SeveralInnerFunctionsFeedAnOuterOneOfThatArity) {
  // Cn[addition; P^2_1, P^2_2](x, y) = x + y
  FunctionPtr f = FunctionLibrary::Composition(
      FunctionLibrary::Addition(),
      {FunctionLibrary::Projection(1, 2), FunctionLibrary::Projection(2, 2)});
  CallCounter counter;
  EXPECT_EQ(f->Evaluate({3, 4}, counter), 7u);
}

TEST(CompositionFunctionTest, CountsItselfItsInnerFunctionsAndTheOuterOne) {
  // Cn[S; P^1_1]: composition + projection + successor = 3 calls.
  FunctionPtr f = FunctionLibrary::Composition(FunctionLibrary::Successor(),
                                               {FunctionLibrary::Projection(1, 1)});
  CallCounter counter;
  f->Evaluate({1}, counter);
  EXPECT_EQ(counter.Count(), 3u);
}

TEST(CompositionFunctionTest, Description) {
  FunctionPtr f = FunctionLibrary::Composition(
      FunctionLibrary::Addition(),
      {FunctionLibrary::Projection(1, 2), FunctionLibrary::Projection(2, 2)});
  EXPECT_EQ(f->Describe(),
            "Cn[Pr[P^1_1, Cn[S; P^3_3]]; P^2_1, P^2_2]");
}

TEST(CompositionFunctionTest, NullFunctionsAreInvalid) {
  EXPECT_THROW(FunctionLibrary::Composition(nullptr, {FunctionLibrary::Zero()}),
               InvalidFunctionException);
  EXPECT_THROW(
      FunctionLibrary::Composition(FunctionLibrary::Successor(), {nullptr}),
      InvalidFunctionException);
}

TEST(CompositionFunctionTest, NoInnerFunctionsIsInvalid) {
  EXPECT_THROW(FunctionLibrary::Composition(FunctionLibrary::Successor(), {}),
               InvalidFunctionException);
}

TEST(CompositionFunctionTest, InnerFunctionsWithDifferentAritiesAreInvalid) {
  EXPECT_THROW(
      FunctionLibrary::Composition(
          FunctionLibrary::Addition(),
          {FunctionLibrary::Projection(1, 2), FunctionLibrary::Projection(1, 3)}),
      InvalidFunctionException);
}

TEST(CompositionFunctionTest, OuterArityMustMatchTheNumberOfInnerFunctions) {
  EXPECT_THROW(
      FunctionLibrary::Composition(FunctionLibrary::Addition(),
                                   {FunctionLibrary::Projection(1, 2)}),
      InvalidFunctionException);
  EXPECT_THROW(
      FunctionLibrary::Composition(
          FunctionLibrary::Successor(),
          {FunctionLibrary::Projection(1, 2), FunctionLibrary::Projection(2, 2)}),
      InvalidFunctionException);
}

// ---- Primitive recursion ----

TEST(PrimitiveRecursionFunctionTest, BaseCaseReturnsTheBaseFunction) {
  FunctionPtr add = FunctionLibrary::Addition();
  CallCounter counter;
  EXPECT_EQ(add->Evaluate({9, 0}, counter), 9u);
}

TEST(PrimitiveRecursionFunctionTest, RecursiveCaseAppliesTheStepFunction) {
  FunctionPtr add = FunctionLibrary::Addition();
  CallCounter counter;
  EXPECT_EQ(add->Evaluate({9, 5}, counter), 14u);
}

TEST(PrimitiveRecursionFunctionTest, ArityIsTheBaseArityPlusOne) {
  EXPECT_EQ(FunctionLibrary::Addition()->Arity(), 2u);
  // A recursion with a 2-variable base has 3 variables.
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Projection(1, 2), FunctionLibrary::Projection(4, 4));
  EXPECT_EQ(f->Arity(), 3u);
}

TEST(PrimitiveRecursionFunctionTest, StepReceivesParametersCounterAndPreviousValue) {
  // f(x, 0) = 0; f(x, y + 1) = P^3_2(x, y, f(x, y)) = y  ->  f(x, y) = y - 1
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(2, 3));
  CallCounter counter;
  EXPECT_EQ(f->Evaluate({7, 1}, counter), 0u);
  EXPECT_EQ(f->Evaluate({7, 6}, counter), 5u);
  // f(x, y + 1) = P^3_1 = x
  FunctionPtr g = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(1, 3));
  EXPECT_EQ(g->Evaluate({7, 3}, counter), 7u);
}

TEST(PrimitiveRecursionFunctionTest, CountsOneCallOfFPerUnitOfTheVariable) {
  // f(x, y) with g = Z^1 and h = P^3_3 (always the previous value):
  // y + 1 calls of f, 1 of g and y of h = 2y + 2.
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(3, 3));
  for (Natural y : {0u, 1u, 5u, 20u}) {
    CallCounter counter;
    f->Evaluate({3, y}, counter);
    EXPECT_EQ(counter.Count(), 2 * y + 2) << y;
  }
}

TEST(PrimitiveRecursionFunctionTest, LargeVariableDoesNotOverflowTheStack) {
  FunctionPtr f = FunctionLibrary::PrimitiveRecursion(
      FunctionLibrary::Zero(1), FunctionLibrary::Projection(3, 3));
  CallCounter counter(10'000'000);
  EXPECT_EQ(f->Evaluate({1, 1'000'000}, counter), 0u);
}

TEST(PrimitiveRecursionFunctionTest, Description) {
  EXPECT_EQ(FunctionLibrary::Addition()->Describe(),
            "Pr[P^1_1, Cn[S; P^3_3]]");
}

TEST(PrimitiveRecursionFunctionTest, NullFunctionsAreInvalid) {
  EXPECT_THROW(FunctionLibrary::PrimitiveRecursion(nullptr, FunctionLibrary::Zero(3)),
               InvalidFunctionException);
  EXPECT_THROW(FunctionLibrary::PrimitiveRecursion(FunctionLibrary::Zero(1), nullptr),
               InvalidFunctionException);
}

TEST(PrimitiveRecursionFunctionTest, StepArityMustBeBasePlusTwo) {
  EXPECT_THROW(FunctionLibrary::PrimitiveRecursion(FunctionLibrary::Zero(1),
                                                   FunctionLibrary::Zero(2)),
               InvalidFunctionException);
  EXPECT_THROW(FunctionLibrary::PrimitiveRecursion(FunctionLibrary::Zero(1),
                                                   FunctionLibrary::Zero(4)),
               InvalidFunctionException);
}

TEST(PrimitiveRecursionFunctionTest, EvaluatingWithTheWrongArityThrows) {
  CallCounter counter;
  EXPECT_THROW(FunctionLibrary::Addition()->Evaluate({1}, counter),
               InvalidArityException);
}

TEST(PrimitiveRecursionFunctionTest, OverflowInsideTheRecursionPropagates) {
  // f(x, y) = x + y with x = max and y = 1 needs successor(max).
  FunctionPtr add = FunctionLibrary::Addition();
  CallCounter counter;
  Natural max = std::numeric_limits<Natural>::max();
  EXPECT_THROW(add->Evaluate({max, 1}, counter), NaturalOverflowException);
}

TEST(PrimitiveRecursionFunctionTest, CallLimitStopsALongRecursion) {
  FunctionPtr add = FunctionLibrary::Addition();
  CallCounter counter(50);
  EXPECT_THROW(add->Evaluate({1, 1000}, counter), CallLimitExceededException);
}
