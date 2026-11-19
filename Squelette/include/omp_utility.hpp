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
  int adresse_nb = std::bit_cast<std::uintptr_t>(&*it);
  if ((adresse_nb & std::hardware_destructive_interference_size) == 0)
    return true;
  return false;
}

template <typename T>
constexpr std::size_t how_many_in_cache_line(void) noexcept {
  // À COMPLÉTER (la valeur retournée ici n'est pas la bonne).
  return 0;
}

} // namespace utility

} // namespace omp_algorithm

#endif