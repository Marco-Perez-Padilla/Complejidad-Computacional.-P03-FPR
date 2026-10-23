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

** File call_counter.h: counter of function calls.
**/

#ifndef CALL_COUNTER_H_
#define CALL_COUNTER_H_

#include <cstdint>

class CallCounter {
 public:
  static constexpr std::uint64_t kDefaultLimit = 50'000'000;

  explicit CallCounter(std::uint64_t limit = kDefaultLimit);

  /**
   * @brief Registers one more function call.
   * @throws CallLimitExceededException if this call goes over the limit.
   */
  void Increment();

  std::uint64_t Count() const { return count_; }
  std::uint64_t Limit() const { return limit_; }

 private:
  std::uint64_t count_ = 0;
  std::uint64_t limit_;
};

#endif
