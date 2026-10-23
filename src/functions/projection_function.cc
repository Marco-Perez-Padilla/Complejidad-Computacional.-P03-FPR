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

** File projection_function.cc: implementation of ProjectionFunction.
**/

#include "functions/projection_function.h"

#include "exceptions/exceptions.h"


/**
 * @brief Constructs a projection function P^k_i(x_1, ..., x_k) = x_i.
 * @param index Position i (starting at 1) of the argument returned.
 * @param arity Number of variables k.
 * @throws InvalidFunctionException unless 1 <= index <= arity.
 */
ProjectionFunction::ProjectionFunction(std::size_t index, std::size_t arity)
    : index_(index), arity_(arity) {
  if (index < 1 || index > arity) {
    throw InvalidFunctionException(
        "projection P^" + std::to_string(arity) + "_" + std::to_string(index) +
        " needs 1 <= index <= arity");
  }
}

/**
 * @brief Returns a string representation of the projection function.
 * @return A string representation of the projection function.
 */
std::string ProjectionFunction::Describe() const {
  return "P^" + std::to_string(arity_) + "_" + std::to_string(index_);
}


/**
 * @brief Evaluates the projection function on the given arguments.
 * @param args The arguments to evaluate the function on.
 * @param counter The call counter to count the number of function calls.
 * @return The result of evaluating the projection function on the given arguments.
 */
Natural ProjectionFunction::DoEvaluate(const Arguments& args,
                                       CallCounter&) const {
  return args[index_ - 1];
}
