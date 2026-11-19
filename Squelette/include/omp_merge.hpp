#ifndef OMP_MERGE_HPP
#define OMP_MERGE_HPP

#include <omp.h>

#include <cmath>
#include <concepts>
#include <iterator>
#include <utility>

namespace omp_algorithm {

/**
 * @brief Parallel version of merge algorithm that Combines the elements in
 * the sorted ranges [first1,last1) and [first2,last2), into a new range
 * beginning at result with all its elements sorted.
 *
 * The elements are compared using operator< for the first version, and comp
 * for the second. The elements in both ranges shall already be ordered
 * according to this same criterion (operator< or comp). The resulting range
 * is also sorted according to this.
 */
namespace omp_merge {
template <typename Input_Random_Access_Iterator_1,
          typename Input_Random_Access_Iterator_2,
          typename Output_Random_Access_Iterator, typename Compare>
Output_Random_Access_Iterator
merge_array_impl(Input_Random_Access_Iterator_1 first_1,
                 Input_Random_Access_Iterator_1 last_1,
                 Input_Random_Access_Iterator_2 first_2,
                 Input_Random_Access_Iterator_2 last_2,
                 Output_Random_Access_Iterator result, Compare comp) noexcept {

  using size_type = std::size_t;

  auto co_rank = [](const size_type i, const Input_Random_Access_Iterator_1 &a,
                    const size_type m, const Input_Random_Access_Iterator_2 &b,
                    const size_type n, const Compare &comp, size_type &j,
                    size_type &k) -> void {
    // À COMPLÉTER.
  };

  const size_type m = static_cast<size_type>(last_1 - first_1);
  const size_type n = static_cast<size_type>(last_2 - first_2);
  const size_type total = m + n;

  auto best_block_size = [&result, total](const int num_threads) -> size_type {
    // À COMPLÉTER (la valeur retournée ici n'est pas la bonne).
    return 0;
  };

  // À COMPLÉTER.

  // Comportement classique de merge.
  return result + total;
}

template <typename Input_Iterator_1, typename Input_Iterator_2,
          typename Output_Iterator, typename Compare>
concept merge_precondition = std::input_iterator<Input_Iterator_1> &&
    std::input_iterator<Input_Iterator_2> &&
    std::output_iterator<Output_Iterator,
                         std::iter_value_t<Input_Iterator_1>> &&
    std::output_iterator<Output_Iterator,
                         std::iter_value_t<Input_Iterator_2>> &&
    std::relation<Compare, std::iter_value_t<Input_Iterator_1>,
                  std::iter_value_t<Input_Iterator_2>>;

template <typename Input_Iterator_1, typename Input_Iterator_2,
          typename Output_Iterator, typename Compare>
requires merge_precondition<Input_Iterator_1, Input_Iterator_2, Output_Iterator,
                            Compare>
    Output_Iterator merge(
        Input_Iterator_1 first_1, Input_Iterator_1 last_1,
        Input_Iterator_2 first_2, Input_Iterator_2 last_2,
        Output_Iterator result,
        Compare comp = std::less<const std::iter_value_t<Input_Iterator_1> &>())
noexcept {
  // À COMPLÉTER (la valeur retournée ici n'est pas la bonne).
  return result;
}
} // namespace omp_merge

} // namespace omp_algorithm

#endif