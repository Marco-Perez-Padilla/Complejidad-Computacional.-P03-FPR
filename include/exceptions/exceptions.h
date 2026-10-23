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

** File exceptions.h: exception hierarchy for the primitive recursive
** function calculator.
**/

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <cstddef>
#include <cstdint>
#include <exception>
#include <string>

/**
 * @brief Base class for every exception thrown by the calculator.
 */
class FprException : public std::exception {
 public:
  explicit FprException(const std::string& message) : message_(message) {}

  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

/**
 * @brief Thrown when the command-line arguments are incorrect or
 * incomplete.
 */
class InvalidArgumentsException : public FprException {
 public:
  explicit InvalidArgumentsException(const std::string& reason)
      : FprException("invalid arguments: " + reason) {}
};

/**
 * @brief Thrown when a function is built in a way that violates the
 * definition of the composition or primitive recursion operators (e.g.
 * the component functions do not have compatible arities) or of a
 * basic function (e.g. a projection onto a position that does not
 * exist).
 */
class InvalidFunctionException : public FprException {
 public:
  explicit InvalidFunctionException(const std::string& reason)
      : FprException("invalid function definition: " + reason) {}
};

/**
 * @brief Thrown when a function is evaluated with a number of
 * arguments different from its arity.
 */
class InvalidArityException : public FprException {
 public:
  InvalidArityException(const std::string& function, std::size_t expected,
                        std::size_t received)
      : FprException("function " + function + " expects " +
                     std::to_string(expected) + " argument(s) but received " +
                     std::to_string(received)) {}
};

/**
 * @brief Thrown when a computation needs a natural number larger than
 * the largest one the implementation can represent. The only basic
 * function that can create a larger number is the successor, so that is
 * where it is detected.
 */
class NaturalOverflowException : public FprException {
 public:
  NaturalOverflowException()
      : FprException("the computation exceeds the largest representable "
                     "natural number (" +
                     std::to_string(UINT64_MAX) + ")") {}
};

/**
 * @brief Thrown when a computation makes more function calls than the
 * configured safety limit. Calls grow very quickly with the size of the
 * arguments (the primitive recursive definition of the power is
 * extremely inefficient), so this guards against a run that would last
 * practically forever.
 */
class CallLimitExceededException : public FprException {
 public:
  explicit CallLimitExceededException(std::uint64_t limit)
      : FprException("the computation exceeded the maximum number of "
                     "function calls (" +
                     std::to_string(limit) + "); use -limit to raise it") {}
};

#endif
