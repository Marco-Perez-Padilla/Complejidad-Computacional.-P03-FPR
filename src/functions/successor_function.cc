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

** File successor_function.cc: implementation of SuccessorFunction.
**/

#include "functions/successor_function.h"

#include <limits>

#include "exceptions/exceptions.h"

/**
 * @brief Evaluates the successor function on the given arguments.
 * @param args The arguments to evaluate the function on.
 * @param counter The call counter to count the number of function calls.
 * @return The result of evaluating the successor function on the given arguments.
 * @throws NaturalOverflowException if the argument is the largest natural.
 */
Natural SuccessorFunction::DoEvaluate(const Arguments& args,
                                      CallCounter&) const {
  if (args[0] == std::numeric_limits<Natural>::max()) {
    throw NaturalOverflowException();
  }
  return args[0] + 1;
}
