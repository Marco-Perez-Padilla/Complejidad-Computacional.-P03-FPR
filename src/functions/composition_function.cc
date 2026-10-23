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

** File composition_function.cc: implementation of CompositionFunction.
**/

#include "functions/composition_function.h"

#include <utility>

#include "exceptions/exceptions.h"


/**
 * @brief Constructs a composition function Cn[h; g_1, ..., g_m] from an outer
 * function h and inner functions g_1, ..., g_m.
 * @param outer The outer function h.
 * @param inner The inner functions g_1, ..., g_m.
 * @throws InvalidFunctionException if a function is null, there are no
 *         g_i, the g_i do not all have the same arity, or h's arity is not the number of g_i.
 */
CompositionFunction::CompositionFunction(FunctionPtr outer,
                                         std::vector<FunctionPtr> inner)
    : outer_(std::move(outer)), inner_(std::move(inner)), arity_(0) {
  if (!outer_) throw InvalidFunctionException("composition with a null function");
  if (inner_.empty()) {
    throw InvalidFunctionException("composition needs at least one inner function");
  }
  for (const FunctionPtr& function : inner_) {
    if (!function) throw InvalidFunctionException("composition with a null function");
  }
  arity_ = inner_.front()->Arity();
  for (const FunctionPtr& function : inner_) {
    if (function->Arity() != arity_) {
      throw InvalidFunctionException(
          "the inner functions of a composition must all have the same arity");
    }
  }
  if (outer_->Arity() != inner_.size()) {
    throw InvalidFunctionException(
        "the outer function " + outer_->Describe() + " takes " +
        std::to_string(outer_->Arity()) + " argument(s) but " +
        std::to_string(inner_.size()) + " inner function(s) were given");
  }
}


/**
 * @brief Returns a string representation of the composition function.
 * @return A string representation of the composition function.
 */
std::string CompositionFunction::Describe() const {
  std::string text = "Cn[" + outer_->Describe() + ";";
  for (std::size_t i = 0; i < inner_.size(); ++i) {
    text += (i == 0 ? " " : ", ") + inner_[i]->Describe();
  }
  return text + "]";
}


/**
 * @brief Evaluates the composition function on the given arguments.
 * @param args The arguments to evaluate the function on.
 * @param counter The call counter to count the number of function calls.
 * @return The result of evaluating the composition function on the given arguments.
 */
Natural CompositionFunction::DoEvaluate(const Arguments& args,
                                        CallCounter& counter) const {
  Arguments values;
  values.reserve(inner_.size());
  for (const FunctionPtr& function : inner_) {
    values.push_back(function->Evaluate(args, counter));
  }
  return outer_->Evaluate(values, counter);
}
