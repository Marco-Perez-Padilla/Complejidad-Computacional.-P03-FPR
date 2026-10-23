// File help_functions_test.cc: tests for the user-facing message
// functions, natural number parsing and command-line argument
// validation.

#include "help/help_functions.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace {

struct Result {
  int status;
  Options options;
  std::string out;
  std::string err;
};

// Calls ValidateArguments with the given arguments (after the program
// name), capturing everything it prints.
Result Validate(std::vector<std::string> args) {
  std::vector<std::string> storage = {"fpr"};
  storage.insert(storage.end(), args.begin(), args.end());
  std::vector<char*> argv;
  for (std::string& arg : storage) argv.push_back(arg.data());

  Result result;
  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();
  result.status = ValidateArguments(static_cast<int>(argv.size()), argv.data(),
                                    &result.options);
  result.err = testing::internal::GetCapturedStderr();
  result.out = testing::internal::GetCapturedStdout();
  return result;
}

}  // namespace

// ---- Messages ----

TEST(HelpFunctionsTest, HelpPrintsUsageSummary) {
  testing::internal::CaptureStdout();
  Help();
  std::string out = testing::internal::GetCapturedStdout();
  EXPECT_NE(out.find("-x"), std::string::npos);
  EXPECT_NE(out.find("-y"), std::string::npos);
  EXPECT_NE(out.find("-limit"), std::string::npos);
}

TEST(HelpFunctionsTest, UsageDescribesTheOptionsAndAnExample) {
  testing::internal::CaptureStdout();
  Usage();
  std::string out = testing::internal::GetCapturedStdout();
  EXPECT_NE(out.find("Base of the power"), std::string::npos);
  EXPECT_NE(out.find("Exponent of the power"), std::string::npos);
  EXPECT_NE(out.find("./fpr -x 2 -y 10"), std::string::npos);
}

TEST(HelpFunctionsTest, PrintWarningWritesWithPrefixToStderr) {
  testing::internal::CaptureStderr();
  PrintWarning("careful");
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "Warning: careful\n");
}

TEST(HelpFunctionsTest, PrintErrorWritesWithPrefixToStderr) {
  testing::internal::CaptureStderr();
  PrintError("broken");
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "Error: broken\n");
}

// ---- ParseNatural ----

TEST(ParseNaturalTest, AcceptsPlainDecimalNumbers) {
  Natural value = 99;
  EXPECT_TRUE(ParseNatural("0", &value));
  EXPECT_EQ(value, 0u);
  EXPECT_TRUE(ParseNatural("42", &value));
  EXPECT_EQ(value, 42u);
  EXPECT_TRUE(ParseNatural("007", &value));
  EXPECT_EQ(value, 7u);
}

TEST(ParseNaturalTest, AcceptsTheLargestNatural) {
  Natural value = 0;
  EXPECT_TRUE(ParseNatural("18446744073709551615", &value));
  EXPECT_EQ(value, 18446744073709551615ull);
}

TEST(ParseNaturalTest, RejectsNumbersThatDoNotFit) {
  Natural value = 5;
  EXPECT_FALSE(ParseNatural("18446744073709551616", &value));
  EXPECT_FALSE(ParseNatural("99999999999999999999999", &value));
  EXPECT_EQ(value, 5u);  // untouched on failure
}

TEST(ParseNaturalTest, RejectsAnythingThatIsNotOnlyDigits) {
  Natural value = 0;
  for (const char* text : {"", "-1", "+1", "1.5", "1e3", "abc", "12a", " 12", "12 ", "0x10"}) {
    EXPECT_FALSE(ParseNatural(text, &value)) << "'" << text << "'";
  }
}

// ---- ValidateArguments ----

TEST(ValidateArgumentsTest, XAndYAreParsed) {
  Result result = Validate({"-x", "2", "-y", "10"});
  EXPECT_EQ(result.status, -1);
  EXPECT_EQ(result.options.base, 2u);
  EXPECT_EQ(result.options.exponent, 10u);
  EXPECT_EQ(result.options.max_calls, CallCounter::kDefaultLimit);
}

TEST(ValidateArgumentsTest, OptionsCanComeInAnyOrder) {
  Result result = Validate({"-y", "3", "-x", "5"});
  EXPECT_EQ(result.status, -1);
  EXPECT_EQ(result.options.base, 5u);
  EXPECT_EQ(result.options.exponent, 3u);
}

TEST(ValidateArgumentsTest, ZeroValuesAreAllowed) {
  Result result = Validate({"-x", "0", "-y", "0"});
  EXPECT_EQ(result.status, -1);
}

TEST(ValidateArgumentsTest, LimitIsParsed) {
  Result result = Validate({"-x", "2", "-y", "3", "-limit", "1000"});
  EXPECT_EQ(result.status, -1);
  EXPECT_EQ(result.options.max_calls, 1000u);
}

TEST(ValidateArgumentsTest, HelpFlagsPrintHelpAndReturnZero) {
  for (const char* flag : {"--help", "-h"}) {
    Result result = Validate({flag});
    EXPECT_EQ(result.status, 0) << flag;
    EXPECT_NE(result.out.find("Primitive recursive power function"), std::string::npos);
  }
}

TEST(ValidateArgumentsTest, HelpTakesPriorityOverOtherProblems) {
  EXPECT_EQ(Validate({"-bogus", "--help"}).status, 0);
}

TEST(ValidateArgumentsTest, NoArgumentsIsAnError) {
  Result result = Validate({});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-x <n> is required"), std::string::npos);
}

TEST(ValidateArgumentsTest, MissingYIsAnError) {
  Result result = Validate({"-x", "2"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-y <n> is required"), std::string::npos);
  EXPECT_NE(result.out.find("Command-line options"), std::string::npos);
}

TEST(ValidateArgumentsTest, MissingXIsAnError) {
  Result result = Validate({"-y", "2"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-x <n> is required"), std::string::npos);
}

TEST(ValidateArgumentsTest, OptionWithoutValueIsAnError) {
  Result result = Validate({"-x"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-x requires a value"), std::string::npos);
  EXPECT_NE(Validate({"-x", "1", "-y"}).err.find("-y requires a value"), std::string::npos);
  EXPECT_NE(Validate({"-x", "1", "-y", "1", "-limit"}).err.find("-limit requires a value"),
            std::string::npos);
}

TEST(ValidateArgumentsTest, NegativeNumbersAreRejected) {
  Result result = Validate({"-x", "-3", "-y", "2"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("'-3' is not a valid natural number for -x"),
            std::string::npos);
}

TEST(ValidateArgumentsTest, NonNumericValuesAreRejected) {
  Result result = Validate({"-x", "2", "-y", "abc"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("'abc' is not a valid natural number for -y"),
            std::string::npos);
}

TEST(ValidateArgumentsTest, DecimalValuesAreRejected) {
  EXPECT_EQ(Validate({"-x", "2.5", "-y", "2"}).status, 1);
}

TEST(ValidateArgumentsTest, ValuesTooBigForANaturalAreRejected) {
  EXPECT_EQ(Validate({"-x", "18446744073709551616", "-y", "2"}).status, 1);
}

TEST(ValidateArgumentsTest, AnOptionNameWhereTheValueShouldBeIsRejected) {
  EXPECT_EQ(Validate({"-x", "-y", "3"}).status, 1);
}

TEST(ValidateArgumentsTest, RepeatedOptionIsAnError) {
  Result result = Validate({"-x", "1", "-x", "2", "-y", "3"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-x given more than once"), std::string::npos);
  EXPECT_EQ(Validate({"-x", "1", "-y", "1", "-y", "2"}).status, 1);
  EXPECT_EQ(Validate({"-x", "1", "-y", "1", "-limit", "5", "-limit", "6"}).status, 1);
}

TEST(ValidateArgumentsTest, ZeroLimitIsAnError) {
  Result result = Validate({"-x", "1", "-y", "1", "-limit", "0"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("-limit must be a positive number"), std::string::npos);
}

TEST(ValidateArgumentsTest, UnknownOptionIsAnError) {
  Result result = Validate({"-x", "1", "-y", "1", "-bogus"});
  EXPECT_EQ(result.status, 1);
  EXPECT_NE(result.err.find("unknown option '-bogus'"), std::string::npos);
}

TEST(ValidateArgumentsTest, PositionalArgumentsAreRejected) {
  EXPECT_EQ(Validate({"2", "10"}).status, 1);
}
