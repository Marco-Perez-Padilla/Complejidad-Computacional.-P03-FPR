/**
** Universidad de La Laguna
** Escuela Superior de Ingenieria y Tecnologia
** Grado en Ingenieria Informatica
** Subject: 'Complejidad Computacional'
** Course: 4º
** Practice 3: Primitive recursive functions
** Author: Marco Pérez Padilla
** Email: alu0101469348@ull.edu.es
** Date: 20/10/2026

** File help_functions.cc: functions that present messages to the user
** (help, usage, and non-exception warnings/errors) and validate the
** command-line arguments.
**/

#include "help/help_functions.h"

#include <iostream>
#include <limits>

/**
 * @brief Prints a short summary of the program and its options.
 */
void Help() {
  std::cout
      << "Primitive recursive power function\n\n"
      << "Usage: ./fpr -x <n> -y <n> [-limit <n>]\n\n"
      << "Options:\n"
      << "  -x <n>        Base of the power (natural number, required)\n"
      << "  -y <n>        Exponent of the power (natural number, required)\n"
      << "  -limit <n>    Maximum number of function calls (optional;\n"
      << "                50000000 by default)\n"
      << "  --help, -h    Show this message and exit\n";
}

/**
 * @brief Prints a detailed description of the program and its
 * command-line options. Called when the arguments provided are
 * incorrect.
 */
void Usage() {
  std::cout
      << "Primitive recursive power function\n\n"
      << "Computes power(x, y) = x^y as a primitive recursive function,\n"
      << "built only from the basic functions (zero, successor and\n"
      << "projections) with the composition and primitive recursion\n"
      << "operators, and shows the result and the number of function\n"
      << "calls made.\n\n"
      << "Command-line options:\n"
      << "  -x <n>        Base of the power: a natural number\n"
      << "                (0 to 18446744073709551615). Required.\n"
      << "  -y <n>        Exponent of the power: a natural number.\n"
      << "                Required.\n"
      << "  -limit <n>    Maximum number of function calls before the\n"
      << "                computation is stopped (a positive number;\n"
      << "                50000000 by default). The primitive recursive\n"
      << "                power makes a huge number of calls, so big\n"
      << "                values need a bigger limit.\n"
      << "  --help, -h    Show a short help message and exit.\n\n"
      << "Example: ./fpr -x 2 -y 10\n";
}

/**
 * @brief Prints a non-critical warning to stderr, with a uniform
 * prefix. A warning does not stop program execution.
 * @param message Description of the warning.
 */
void PrintWarning(const std::string& message) {
  std::cerr << "Warning: " << message << '\n';
}

/**
 * @brief Prints a critical error to stderr, with a uniform prefix. The
 * caller decides whether the program should terminate.
 * @param message Description of the error.
 */
void PrintError(const std::string& message) {
  std::cerr << "Error: " << message << '\n';
}

/**
 * @brief Converts text to a natural number.
 * @param text Only decimal digits are accepted: no sign, no spaces, no
 *        decimals, not empty.
 * @param value Receives the number when the text is valid.
 * @return true if text is a natural number that fits in a Natural.
 */
bool ParseNatural(const std::string& text, Natural* value) {
  if (text.empty()) return false;
  Natural result = 0;
  for (char c : text) {
    if (c < '0' || c > '9') return false;
    Natural digit = static_cast<Natural>(c - '0');
    if (result > (std::numeric_limits<Natural>::max() - digit) / 10) {
      return false;
    }
    result = result * 10 + digit;
  }
  *value = result;
  return true;
}

namespace {

// Reads the value of option `name` (the argument after argv[*i]) as a
// natural number, advancing *i. Prints an error and Usage() and returns
// false when the value is missing or is not a natural number.
bool ReadNaturalOption(const std::string& name, int argc, char* argv[], int* i,
                       Natural* value) {
  if (*i + 1 >= argc) {
    PrintError(name + " requires a value");
    Usage();
    return false;
  }
  std::string text = argv[++*i];
  if (!ParseNatural(text, value)) {
    PrintError("'" + text + "' is not a valid natural number for " + name);
    Usage();
    return false;
  }
  return true;
}

}  // namespace

/**
 * @brief Parses and validates argv into options.
 * @param argc Argument count, as received by main.
 * @param argv Argument vector, as received by main.
 * @param options Filled in with the parsed options on success.
 * @return 0 if --help/-h was requested (Help() was already printed);
 *         -1 if the arguments are correct and options is ready to use;
 *          1 if the arguments are incorrect (an error and Usage() were
 *         already printed).
 */
int ValidateArguments(int argc, char* argv[], Options* options) {
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      Help();
      return 0;
    }
  }

  bool has_x = false;
  bool has_y = false;
  bool has_limit = false;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    bool* seen = nullptr;
    Natural* target = nullptr;
    Natural limit = 0;
    if (arg == "-x") {
      seen = &has_x;
      target = &options->base;
    } else if (arg == "-y") {
      seen = &has_y;
      target = &options->exponent;
    } else if (arg == "-limit") {
      seen = &has_limit;
      target = &limit;
    } else {
      PrintError("unknown option '" + arg + "'");
      Usage();
      return 1;
    }
    if (*seen) {
      PrintError(arg + " given more than once");
      Usage();
      return 1;
    }
    if (!ReadNaturalOption(arg, argc, argv, &i, target)) return 1;
    *seen = true;
    if (arg == "-limit") {
      if (limit == 0) {
        PrintError("-limit must be a positive number");
        Usage();
        return 1;
      }
      options->max_calls = limit;
    }
  }

  if (!has_x || !has_y) {
    PrintError(std::string(!has_x ? "-x <n>" : "-y <n>") + " is required");
    Usage();
    return 1;
  }
  return -1;
}
