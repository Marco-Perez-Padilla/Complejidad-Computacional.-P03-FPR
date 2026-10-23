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

** File call_counter.cc: implementation of CallCounter.
**/

#include "functions/call_counter.h"

#include "exceptions/exceptions.h"


/**
 * @brief Constructs a call counter with the given limit.
 * @param limit The maximum number of function calls allowed.
 */
CallCounter::CallCounter(std::uint64_t limit) : limit_(limit) {}


/**
 * @brief Counts the function calls made during a computation and stops
 * it when a safety limit is exceeded.
 *
 * It is passed explicitly through every evaluation instead of being a
 * global, so each computation has its own count and the functions
 * themselves stay stateless (the same function object can be evaluated
 * any number of times, and shared between several definitions).
 */
void CallCounter::Increment() {
  if (count_ >= limit_) throw CallLimitExceededException(limit_);
  ++count_;
}
