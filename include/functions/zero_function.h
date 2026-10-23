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

** File zero_function.h: the basic function zero.
**/

#ifndef ZERO_FUNCTION_H_
#define ZERO_FUNCTION_H_

#include "functions/function.h"

/**
 * @brief Basic function Z^k(x_1, ..., x_k) = 0. Ignores its arguments.
 */
class ZeroFunction : public Function {
 public:
  explicit ZeroFunction(std::size_t arity = 1);

  std::size_t Arity() const override { return arity_; }
  std::string Describe() const override;

 protected:
  Natural DoEvaluate(const Arguments& args,
                     CallCounter& counter) const override;

 private:
  std::size_t arity_;
};

#endif
