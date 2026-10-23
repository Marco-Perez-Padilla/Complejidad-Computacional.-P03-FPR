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

** File zero_function.cc: implementation of ZeroFunction.
**/

#include "functions/zero_function.h"

#include "exceptions/exceptions.h"


/**
 * @brief Constructs a zero function Z^k(x_1, ..., x_k) = 0.
 * @param arity Number of variables k.
 * @throws InvalidFunctionException if arity is 0.
 */
ZeroFunction::ZeroFunction(std::size_t arity) : arity_(arity) {
  if (arity == 0) {
    throw InvalidFunctionException("the zero function needs at least 1 variable");
  }
}

/**
 * @brief Returns a string describing the zero function.
 * @return A string describing the zero function.
 */
std::string ZeroFunction::Describe() const {
  return "Z^" + std::to_string(arity_);
}


/**
 * @brief Evaluates the zero function on the given arguments.
 * @param args The arguments to evaluate the function on.
 * @param counter The call counter to count the number of function calls.
 * @return The result of evaluating the zero function on the given arguments.
 *         Always returns 0.
 */
Natural ZeroFunction::DoEvaluate(const Arguments&, CallCounter&) const {
  return 0;
}
