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

** File options.h: command-line options accepted by the calculator.
**/

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "functions/call_counter.h"
#include "model/natural.h"

/**
 * @brief Command-line options accepted by the calculator, filled in by
 * ValidateArguments (see help/help_functions.h).
 */
struct Options {
  Natural base = 0;      // -x
  Natural exponent = 0;  // -y
  std::uint64_t max_calls = CallCounter::kDefaultLimit;  // -limit
};

#endif
