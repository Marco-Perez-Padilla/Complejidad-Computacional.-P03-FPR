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

** File function_library.cc: implementation of FunctionLibrary.
**/

#include "functions/function_library.h"

#include <utility>

#include "functions/composition_function.h"
#include "functions/primitive_recursion_function.h"
#include "functions/projection_function.h"
#include "functions/successor_function.h"
#include "functions/zero_function.h"

/**
 * @brief Builds the basic functions and the primitive recursive
 * functions defined from them, each one only through the basic functions
 * and the composition and primitive recursion operators.
 * @param arity The number of variables the function takes.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Zero(std::size_t arity) {
  return std::make_shared<ZeroFunction>(arity);
}


/**
 * @brief Builds the successor function S(x) = x + 1.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Successor() {
  return std::make_shared<SuccessorFunction>();
}


/**
 * @brief Builds the projection function P^k_i(x_1, ..., x_k) = x_i.
 * @param index The index of the variable to project.
 * @param arity The number of variables the function takes.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Projection(std::size_t index, std::size_t arity) {
  return std::make_shared<ProjectionFunction>(index, arity);
}


/**
 * @brief Builds the composition function f(g_1(x), ..., g_n(x)).
 * @param outer The outer function f.
 * @param inner The vector of inner functions g_1, ..., g_n.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Composition(FunctionPtr outer,
                                         std::vector<FunctionPtr> inner) {
  return std::make_shared<CompositionFunction>(std::move(outer),
                                               std::move(inner));
}


/**
 * @brief Builds the primitive recursion function.
 * @param base The base function.
 * @param step The step function.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::PrimitiveRecursion(FunctionPtr base,
                                                FunctionPtr step) {
  return std::make_shared<PrimitiveRecursionFunction>(std::move(base),
                                                      std::move(step));
}


/**
 * @brief Builds the addition function.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Addition() {
  FunctionPtr step = Composition(Successor(), {Projection(3, 3)});
  return PrimitiveRecursion(Projection(1, 1), step);
}


/**
 * @brief Builds the multiplication function.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Multiplication() {
  FunctionPtr step = Composition(Addition(), {Projection(3, 3), Projection(1, 3)});
  return PrimitiveRecursion(Zero(1), step);
}


/**
 * @brief Builds the power function.
 * @return A shared pointer to the constructed function.
 */
FunctionPtr FunctionLibrary::Power() {
  FunctionPtr base = Composition(Successor(), {Zero(1)});
  FunctionPtr step =
      Composition(Multiplication(), {Projection(3, 3), Projection(1, 3)});
  return PrimitiveRecursion(base, step);
}
