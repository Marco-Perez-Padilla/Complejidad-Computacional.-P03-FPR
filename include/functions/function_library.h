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

** File function_library.h: the functions built from the basic ones.
**/

#ifndef FUNCTION_LIBRARY_H_
#define FUNCTION_LIBRARY_H_

#include <vector>

#include "functions/function.h"

/**
 * @brief Builds the basic functions and the primitive recursive
 * functions defined from them, each one only through the basic functions
 * and the composition and primitive recursion operators.
 *
 * They are built as trees of function objects (not as subclasses that
 * wrap one), so every node of the definition is evaluated, and counted,
 * exactly once per call.
 */
class FunctionLibrary {
 public:
  /// Z^arity(x_1, ..., x_arity) = 0
  static FunctionPtr Zero(std::size_t arity = 1);

  /// S(x) = x + 1
  static FunctionPtr Successor();

  /// P^arity_index(x_1, ..., x_arity) = x_index
  static FunctionPtr Projection(std::size_t index, std::size_t arity);

  /// Cn[outer; inner_1, ..., inner_m]
  static FunctionPtr Composition(FunctionPtr outer,
                                 std::vector<FunctionPtr> inner);

  /// Pr[base, step]
  static FunctionPtr PrimitiveRecursion(FunctionPtr base, FunctionPtr step);

  /**
   * addition(x, y) = x + y
   *   addition(x, 0)     = P^1_1(x)
   *   addition(x, y + 1) = S(P^3_3(x, y, addition(x, y)))
   * = Pr[P^1_1, Cn[S; P^3_3]]
   */
  static FunctionPtr Addition();

  /**
   * multiplication(x, y) = x * y
   *   multiplication(x, 0)     = Z^1(x)
   *   multiplication(x, y + 1) = addition(multiplication(x, y), x)
   * = Pr[Z^1, Cn[addition; P^3_3, P^3_1]]
   */
  static FunctionPtr Multiplication();

  /**
   * power(x, y) = x^y
   *   power(x, 0)     = S(Z^1(x))
   *   power(x, y + 1) = multiplication(power(x, y), x)
   * = Pr[Cn[S; Z^1], Cn[multiplication; P^3_3, P^3_1]]
   */
  static FunctionPtr Power();
};

#endif
