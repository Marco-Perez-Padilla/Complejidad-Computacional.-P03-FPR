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

** File help_functions.h: functions that present messages to the user
** (help, usage, and non-exception warnings/errors) and validate the
** command-line arguments.
**/

#ifndef HELP_FUNCTIONS_H_
#define HELP_FUNCTIONS_H_

#include <string>

#include "io/options.h"
#include "model/natural.h"

void Help();
void Usage();
int ValidateArguments(int argc, char* argv[], Options* options);
bool ParseNatural(const std::string& text, Natural* value);
void PrintWarning(const std::string& message);
void PrintError(const std::string& message);

#endif
