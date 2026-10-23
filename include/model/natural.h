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

** File natural.h: the type used for natural numbers.
**/

#ifndef NATURAL_H_
#define NATURAL_H_

#include <cstdint>
#include <vector>

/**
 * @brief Natural numbers (0, 1, 2, ...) are represented as unsigned
 * 64-bit integers. Going past the largest one is detected and reported
 * (see NaturalOverflowException).
 */
using Natural = std::uint64_t;

/**
 * @brief Argument list of a function of several natural variables.
 */
using Arguments = std::vector<Natural>;

#endif
