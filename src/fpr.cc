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

** File fpr.cc: Client program that computes the power function
** x^y as a primitive recursive function. It validates the command-line
** arguments and shows the result and the number of function calls.
**/

#include "exceptions/exceptions.h"
#include "help/help_functions.h"
#include "io/application.h"
#include "io/options.h"

int main(int argc, char* argv[]) {
  Options options;
  int status = ValidateArguments(argc, argv, &options);
  if (status != -1) return status;

  try {
    Application(options).Run();
  } catch (const FprException& error) {
    PrintError(error.what());
    return 1;
  }
  return 0;
}
