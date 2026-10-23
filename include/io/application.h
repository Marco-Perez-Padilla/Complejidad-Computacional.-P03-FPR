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

** File application.h: top-level orchestration invoked by main after
** the command-line arguments are validated.
**/

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>

#include "io/options.h"

/**
 * @brief Computes power(x, y) as a primitive recursive function and
 * reports the result and the number of function calls made.
 *
 * The output stream is injected (it defaults to std::cout) so the whole
 * run can be exercised in tests with a string stream.
 */
class Application {
 public:
  explicit Application(Options options, std::ostream& out = std::cout);
  void Run();

 private:
  Options options_;
  std::ostream& out_;
};

#endif
