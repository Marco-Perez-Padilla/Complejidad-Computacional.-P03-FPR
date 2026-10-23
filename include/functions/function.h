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

** File function.h: abstract base class of every primitive recursive
** function.
**/

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <cstddef>
#include <memory>
#include <string>

#include "functions/call_counter.h"
#include "model/natural.h"

/**
 * @brief A function of k natural variables (k is its arity), built from
 * the basic functions with the composition and primitive recursion
 * operators.
 *
 * Every evaluation of any function in the definition counts as one
 * function call, including the basic ones. That count is done here, in
 * the non-virtual Evaluate (Template Method), so no subclass can forget
 * it: subclasses only implement DoEvaluate.
 */
class Function {
 public:
  virtual ~Function() = default;

  Natural Evaluate(const Arguments& args, CallCounter& counter) const;

  /// Number of variables the function takes.
  virtual std::size_t Arity() const = 0;

  /// Notation of the definition, e.g. "Cn[S; P^3_3]".
  virtual std::string Describe() const = 0;

 protected:
  virtual Natural DoEvaluate(const Arguments& args,
                             CallCounter& counter) const = 0;
};

/// Functions are shared: addition is part of multiplication, which is
/// part of the power.
using FunctionPtr = std::shared_ptr<const Function>;

#endif
