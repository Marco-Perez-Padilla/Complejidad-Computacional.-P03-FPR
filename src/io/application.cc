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

** File application.cc: implementation of Application.
**/

#include "io/application.h"

#include <utility>

#include "functions/call_counter.h"
#include "functions/function_library.h"

Application::Application(Options options, std::ostream& out)
    : options_(std::move(options)), out_(out) {}


/**
 * @brief Builds the power function, evaluates it on (x, y) and prints
 * the result and the number of function calls.
 * @throws FprException (or a subclass) if the computation overflows or
 *         exceeds the call limit.
 */
void Application::Run() {
  FunctionPtr power = FunctionLibrary::Power();
  CallCounter counter(options_.max_calls);

  Natural result = power->Evaluate({options_.base, options_.exponent}, counter);

  out_ << "power(" << options_.base << ", " << options_.exponent
       << ") = " << result << '\n'
       << "Function calls: " << counter.Count() << '\n';
}
