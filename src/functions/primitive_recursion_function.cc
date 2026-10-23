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

** File primitive_recursion_function.cc: implementation of
** PrimitiveRecursionFunction.
**/

#include "functions/primitive_recursion_function.h"

#include <utility>

#include "exceptions/exceptions.h"


/**
 * @brief Constructs a primitive recursion function Pr[g, h] from a base
 * function g and a step function h.
 * @param base The base function g.
 * @param step The step function h.
 * @throws InvalidFunctionException if a function is null or h's arity
 *         is not g's arity + 2.
 */
PrimitiveRecursionFunction::PrimitiveRecursionFunction(FunctionPtr base,
                                                       FunctionPtr step)
    : base_(std::move(base)), step_(std::move(step)) {
  if (!base_ || !step_) {
    throw InvalidFunctionException("primitive recursion with a null function");
  }
  if (step_->Arity() != base_->Arity() + 2) {
    throw InvalidFunctionException(
        "in a primitive recursion, the step function " + step_->Describe() +
        " must take " + std::to_string(base_->Arity() + 2) +
        " arguments (the base function's arity + 2) but takes " +
        std::to_string(step_->Arity()));
  }
}

/**
 * @brief Returns a string representation of the primitive recursion function.
 * @return A string representation of the primitive recursion function.
 */
std::string PrimitiveRecursionFunction::Describe() const {
  return "Pr[" + base_->Describe() + ", " + step_->Describe() + "]";
}


/**
 * @brief Evaluates the primitive recursion function on the given arguments.
 * @param args The arguments to evaluate the function on.
 * @param counter The call counter to count the number of function calls.
 * @return The result of evaluating the primitive recursion function on the given arguments.
 */
Natural PrimitiveRecursionFunction::DoEvaluate(const Arguments& args,
                                               CallCounter& counter) const {
  const std::size_t n = args.size() - 1;
  const Natural y = args.back();

  Arguments fixed(args.begin(), args.begin() + n);
  Natural value = base_->Evaluate(fixed, counter);  // f(x, 0)

  // step_arguments = (x_1, ..., x_n, i, f(x, i))
  Arguments step_arguments = fixed;
  step_arguments.push_back(0);
  step_arguments.push_back(0);
  for (Natural i = 0; i < y; ++i) {
    counter.Increment();  // the call of f(x, i), made by f(x, i + 1)
    step_arguments[n] = i;
    step_arguments[n + 1] = value;
    value = step_->Evaluate(step_arguments, counter);  // f(x, i + 1)
  }
  return value;
}
