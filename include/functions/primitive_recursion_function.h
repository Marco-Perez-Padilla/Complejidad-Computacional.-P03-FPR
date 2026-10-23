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

** File primitive_recursion_function.h: the primitive recursion
** operator.
**/

#ifndef PRIMITIVE_RECURSION_FUNCTION_H_
#define PRIMITIVE_RECURSION_FUNCTION_H_

#include "functions/function.h"

/**
 * @brief Primitive recursion on the last variable, from g (n variables)
 * and h (n + 2 variables):
 *
 *   f(x_1, ..., x_n, 0)     = g(x_1, ..., x_n)
 *   f(x_1, ..., x_n, y + 1) = h(x_1, ..., x_n, y, f(x_1, ..., x_n, y))
 *
 * f has n + 1 variables. Written Pr[g, h].
 *
 * The evaluation is a loop from 0 up to y, not a chain of nested C++
 * calls, so a large y cannot overflow the stack. The calls are counted
 * as if the definition had been applied recursively: f(.., y) is
 * y + 1 calls of f, one call of g and y calls of h (plus everything h
 * itself calls).
 */
class PrimitiveRecursionFunction : public Function {
 public:
  PrimitiveRecursionFunction(FunctionPtr base, FunctionPtr step);

  std::size_t Arity() const override { return base_->Arity() + 1; }
  std::string Describe() const override;

 protected:
  Natural DoEvaluate(const Arguments& args,
                     CallCounter& counter) const override;

 private:
  FunctionPtr base_;
  FunctionPtr step_;
};

#endif
