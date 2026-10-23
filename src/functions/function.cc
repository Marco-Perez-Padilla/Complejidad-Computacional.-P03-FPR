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

** File function.cc: implementation of Function.
**/

#include "functions/function.h"

#include "exceptions/exceptions.h"


/**
 * @brief Evaluates the function on the given arguments, counting this
 * evaluation (and all the ones it triggers) in counter.
 * @throws InvalidArityException if args.size() != Arity().
 * @throws NaturalOverflowException, CallLimitExceededException.
 */
Natural Function::Evaluate(const Arguments& args, CallCounter& counter) const {
  if (args.size() != Arity()) {
    throw InvalidArityException(Describe(), Arity(), args.size());
  }
  counter.Increment();
  return DoEvaluate(args, counter);
}
