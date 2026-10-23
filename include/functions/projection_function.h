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

** File projection_function.h: the basic projection functions.
**/

#ifndef PROJECTION_FUNCTION_H_
#define PROJECTION_FUNCTION_H_

#include "functions/function.h"

/**
 * @brief Basic function P^k_i(x_1, ..., x_k) = x_i, with 1 <= i <= k.
 */
class ProjectionFunction : public Function {
 public:
  ProjectionFunction(std::size_t index, std::size_t arity);

  std::size_t Arity() const override { return arity_; }
  std::string Describe() const override;

 protected:
  Natural DoEvaluate(const Arguments& args,
                     CallCounter& counter) const override;

 private:
  std::size_t index_;
  std::size_t arity_;
};

#endif
