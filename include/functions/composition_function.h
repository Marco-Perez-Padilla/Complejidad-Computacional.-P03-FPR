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

** File composition_function.h: the composition operator.
**/

#ifndef COMPOSITION_FUNCTION_H_
#define COMPOSITION_FUNCTION_H_

#include <vector>

#include "functions/function.h"

/**
 * @brief Composition of h (m variables) with g_1, ..., g_m (all of them
 * k variables):
 *
 *   f(x_1, ..., x_k) = h(g_1(x_1, ..., x_k), ..., g_m(x_1, ..., x_k))
 *
 * f has k variables. Written Cn[h; g_1, ..., g_m].
 */
class CompositionFunction : public Function {
 public:
  /**
   * @throws InvalidFunctionException if a function is null, there are no
   *         g_i, the g_i do not all have the same arity, or h's arity is
   *         not the number of g_i.
   */
  CompositionFunction(FunctionPtr outer, std::vector<FunctionPtr> inner);

  std::size_t Arity() const override { return arity_; }
  std::string Describe() const override;

 protected:
  Natural DoEvaluate(const Arguments& args,
                     CallCounter& counter) const override;

 private:
  FunctionPtr outer_;
  std::vector<FunctionPtr> inner_;
  std::size_t arity_;
};

#endif
