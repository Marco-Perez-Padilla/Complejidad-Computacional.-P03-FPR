// File function_library_test.cc: tests for the addition,
// multiplication and power built by FunctionLibrary: values, and the
// exact number of calls against the closed formulas derived by hand
// from their definitions.

#include "functions/function_library.h"

#include <gtest/gtest.h>

#include <cstdint>

#include "exceptions/exceptions.h"

namespace {

Natural Eval(const FunctionPtr& f, Arguments args, std::uint64_t* calls = nullptr) {
  CallCounter counter(1'000'000'000);
  Natural result = f->Evaluate(args, counter);
  if (calls != nullptr) *calls = counter.Count();
  return result;
}

Natural Pow(Natural x, Natural y) {
  Natural result = 1;
  for (Natural i = 0; i < y; ++i) result *= x;
  return result;
}

// Calls made by each function, from its definition:
//   addition(x, y):        4y + 2
//   multiplication(x, y):  2 + y * (4x + 6)
//   power(x, 0) = 4;  power(x, y + 1) = power(x, y) + 6 + 6x + 4x^(y+1)
std::uint64_t AdditionCalls(Natural y) { return 4 * y + 2; }
std::uint64_t MultiplicationCalls(Natural x, Natural y) { return 2 + y * (4 * x + 6); }
std::uint64_t PowerCalls(Natural x, Natural y) {
  std::uint64_t calls = 4;
  for (Natural k = 1; k <= y; ++k) calls += 6 + 6 * x + 4 * Pow(x, k);
  return calls;
}

}  // namespace

TEST(FunctionLibraryTest, ZeroSuccessorAndProjectionAreBuilt) {
  EXPECT_EQ(Eval(FunctionLibrary::Zero(2), {5, 6}), 0u);
  EXPECT_EQ(Eval(FunctionLibrary::Successor(), {5}), 6u);
  EXPECT_EQ(Eval(FunctionLibrary::Projection(2, 2), {5, 6}), 6u);
}

TEST(FunctionLibraryTest, AdditionComputesTheSum) {
  FunctionPtr add = FunctionLibrary::Addition();
  EXPECT_EQ(add->Arity(), 2u);
  for (Natural x = 0; x <= 6; ++x) {
    for (Natural y = 0; y <= 6; ++y) EXPECT_EQ(Eval(add, {x, y}), x + y);
  }
}

TEST(FunctionLibraryTest, MultiplicationComputesTheProduct) {
  FunctionPtr mul = FunctionLibrary::Multiplication();
  EXPECT_EQ(mul->Arity(), 2u);
  for (Natural x = 0; x <= 6; ++x) {
    for (Natural y = 0; y <= 6; ++y) EXPECT_EQ(Eval(mul, {x, y}), x * y);
  }
}

TEST(FunctionLibraryTest, PowerComputesThePower) {
  FunctionPtr power = FunctionLibrary::Power();
  EXPECT_EQ(power->Arity(), 2u);
  for (Natural x = 0; x <= 5; ++x) {
    for (Natural y = 0; y <= 5; ++y) EXPECT_EQ(Eval(power, {x, y}), Pow(x, y)) << x << "^" << y;
  }
}

TEST(FunctionLibraryTest, PowerSpecialCases) {
  FunctionPtr power = FunctionLibrary::Power();
  EXPECT_EQ(Eval(power, {0, 0}), 1u);   // 0^0 = 1 by the recursive definition
  EXPECT_EQ(Eval(power, {0, 5}), 0u);
  EXPECT_EQ(Eval(power, {7, 0}), 1u);
  EXPECT_EQ(Eval(power, {1, 40}), 1u);
  EXPECT_EQ(Eval(power, {7, 1}), 7u);
  EXPECT_EQ(Eval(power, {2, 10}), 1024u);
}

TEST(FunctionLibraryTest, AdditionCallCountMatchesTheFormula) {
  FunctionPtr add = FunctionLibrary::Addition();
  for (Natural x : {0u, 3u}) {
    for (Natural y = 0; y <= 8; ++y) {
      std::uint64_t calls = 0;
      Eval(add, {x, y}, &calls);
      EXPECT_EQ(calls, AdditionCalls(y)) << x << "+" << y;
    }
  }
}

TEST(FunctionLibraryTest, MultiplicationCallCountMatchesTheFormula) {
  FunctionPtr mul = FunctionLibrary::Multiplication();
  for (Natural x = 0; x <= 5; ++x) {
    for (Natural y = 0; y <= 5; ++y) {
      std::uint64_t calls = 0;
      Eval(mul, {x, y}, &calls);
      EXPECT_EQ(calls, MultiplicationCalls(x, y)) << x << "*" << y;
    }
  }
}

TEST(FunctionLibraryTest, PowerCallCountMatchesTheFormula) {
  FunctionPtr power = FunctionLibrary::Power();
  for (Natural x = 0; x <= 4; ++x) {
    for (Natural y = 0; y <= 5; ++y) {
      std::uint64_t calls = 0;
      Eval(power, {x, y}, &calls);
      EXPECT_EQ(calls, PowerCalls(x, y)) << x << "^" << y;
    }
  }
}

TEST(FunctionLibraryTest, PowerCallCountKnownValues) {
  std::uint64_t calls = 0;
  FunctionPtr power = FunctionLibrary::Power();
  Eval(power, {0, 0}, &calls);
  EXPECT_EQ(calls, 4u);
  Eval(power, {2, 10}, &calls);
  EXPECT_EQ(calls, 8368u);
  Eval(power, {3, 4}, &calls);
  EXPECT_EQ(calls, 580u);
}

TEST(FunctionLibraryTest, SameFunctionObjectCanBeEvaluatedManyTimes) {
  FunctionPtr power = FunctionLibrary::Power();
  std::uint64_t first = 0, second = 0;
  Eval(power, {2, 6}, &first);
  Eval(power, {2, 6}, &second);
  EXPECT_EQ(first, second);
}

TEST(FunctionLibraryTest, DescriptionsShowOnlyBasicFunctionsAndOperators) {
  EXPECT_EQ(FunctionLibrary::Multiplication()->Describe(),
            "Pr[Z^1, Cn[Pr[P^1_1, Cn[S; P^3_3]]; P^3_3, P^3_1]]");
  EXPECT_EQ(FunctionLibrary::Power()->Describe(),
            "Pr[Cn[S; Z^1], Cn[Pr[Z^1, Cn[Pr[P^1_1, Cn[S; P^3_3]]; P^3_3, P^3_1]]; "
            "P^3_3, P^3_1]]");
}
