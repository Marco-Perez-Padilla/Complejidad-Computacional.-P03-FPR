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

** File successor_function.h: the basic function successor.
**/

#ifndef SUCCESSOR_FUNCTION_H_
#define SUCCESSOR_FUNCTION_H_

#include "functions/function.h"

/**
 * @brief Basic function S(x) = x + 1.
 *
 * It is the only function that makes numbers bigger, so it is the only
 * place where a result can overflow.
 */
class SuccessorFunction : public Function {
 public:
  std::size_t Arity() const override { return 1; }
  std::string Describe() const override { return "S"; }

 protected:
  Natural DoEvaluate(const Arguments& args,
                     CallCounter& counter) const override;
};

#endif
