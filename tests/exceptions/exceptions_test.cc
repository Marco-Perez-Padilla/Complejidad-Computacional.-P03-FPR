// File exceptions_test.cc: tests for the exception hierarchy.

#include "exceptions/exceptions.h"

#include <gtest/gtest.h>

#include <string>

TEST(ExceptionsTest, EveryExceptionDerivesFromFprException) {
  EXPECT_THROW(throw InvalidArgumentsException("x"), FprException);
  EXPECT_THROW(throw InvalidFunctionException("x"), FprException);
  EXPECT_THROW(throw InvalidArityException("S", 1, 2), FprException);
  EXPECT_THROW(throw NaturalOverflowException(), FprException);
  EXPECT_THROW(throw CallLimitExceededException(5), FprException);
}

TEST(ExceptionsTest, InvalidArgumentsMessage) {
  EXPECT_STREQ(InvalidArgumentsException("bad").what(),
               "invalid arguments: bad");
}

TEST(ExceptionsTest, InvalidFunctionMessage) {
  EXPECT_STREQ(InvalidFunctionException("bad").what(),
               "invalid function definition: bad");
}

TEST(ExceptionsTest, InvalidArityMessageNamesFunctionAndCounts) {
  EXPECT_STREQ(InvalidArityException("S", 1, 3).what(),
               "function S expects 1 argument(s) but received 3");
}

TEST(ExceptionsTest, OverflowMessageMentionsTheLargestNatural) {
  EXPECT_NE(std::string(NaturalOverflowException().what())
                .find("18446744073709551615"),
            std::string::npos);
}

TEST(ExceptionsTest, CallLimitMessageMentionsLimitAndOption) {
  std::string message = CallLimitExceededException(123).what();
  EXPECT_NE(message.find("123"), std::string::npos);
  EXPECT_NE(message.find("-limit"), std::string::npos);
}
