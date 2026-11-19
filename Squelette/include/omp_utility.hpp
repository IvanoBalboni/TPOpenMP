#ifndef OMP_UTILITY_HPP
#define OMP_UTILITY_HPP

#include <bit>
#include <cstdint>
#include <new>

namespace omp_algorithm {

/**
 * @brief Variety of utility tools.
 *
 */
namespace utility {

template <typename Random_Access_Iterator>
requires std::random_access_iterator<Random_Access_Iterator>
bool is_aligned_array(const Random_Access_Iterator &it) noexcept {
  // on cherche a verifier que l'adresse commence bien au 1er bit
  // on passe donc le pointer en uint pour comparer avec hardware_destructive_interference_size -1
  std::uintptr_t adresse_nb = std::bit_cast<std::uintptr_t>(&*it);
  return (adresse_nb & (std::hardware_destructive_interference_size -1) )== 0;
}

template <typename T>
constexpr std::size_t how_many_in_cache_line(void) noexcept {
  return std::hardware_destructive_interference_size / sizeof(T);
}

} // namespace utility

} // namespace omp_algorithm

#endif