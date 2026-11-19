#ifndef OMP_MERGE_HPP
#define OMP_MERGE_HPP

#include <omp.h>

#include <cstddef>
#include <omp_algorithm.hpp>

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
    j = std::min(i, m);
    k = i - j;                                  // invariant : j + k == i
    size_type k_low = 0;
    size_type j_low = (i > n) ? i - n : 0;      // max(0, i - n) sans underflow
    bool active = true;
    while (active) {
      if (j > 0 && k < n && comp(b[k], a[j - 1])) {
      // 1re condition du lemme violée (a[j-1] > b[k]) : on diminue j
      const size_type delta = (j - j_low + 1) / 2;   // ceil((j - j_low) / 2)
      k_low = k;
      j -= delta;
      k += delta;
      }
      else if (k > 0 && j < m && !comp(b[k - 1], a[j])) {
        // 2e condition du lemme violée (b[k-1] >= a[j]) : on diminue k
        const size_type delta = (k - k_low + 1) / 2;   // ceil((k - k_low) / 2)
        j_low = j;
        j += delta;
        k -= delta;
      } 
      else {
        // Aucune condition violée : (j, k) est unique
        active = false;
      }
    }
  };

  const size_type m = static_cast<size_type>(last_1 - first_1);
  const size_type n = static_cast<size_type>(last_2 - first_2);
  const size_type total = m + n;

  auto best_block_size = [&result, total](const int num_threads) -> size_type {
    const size_type p = static_cast<size_type>(num_threads);
    return (total + p - 1) / p;
  };

  // Algo 1 SPMD
  #pragma omp parallel
  {
    const size_type r = static_cast<size_type>(omp_get_thread_num());
    const size_type block = best_block_size(omp_get_num_threads());

    const size_type i_r = std::min(r * block, total);
    const size_type i_r1 = std::min((r + 1) * block, total);

    size_type j_r, k_r, j_r1, k_r1;
    co_rank(i_r, first_1, m, first_2, n, comp, j_r, k_r);
    co_rank(i_r1, first_1, m, first_2, n, comp, j_r1, k_r1);

    // Fusion stable du fragment A[j_r, j_r1) et B[k_r, k_r1) dans C[i_r, i_r1).
    std::merge(first_1 + static_cast<std::ptrdiff_t>(j_r),
               first_1 + static_cast<std::ptrdiff_t>(j_r1),
               first_2 + static_cast<std::ptrdiff_t>(k_r),
               first_2 + static_cast<std::ptrdiff_t>(k_r1),
               result + static_cast<std::ptrdiff_t>(i_r), comp);
  }
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
  if constexpr (std::contiguous_iterator<Input_Iterator_1> &&
                std::contiguous_iterator<Input_Iterator_2> && 
                std::contiguous_iterator<Output_Iterator>){
    return merge_array_impl( first_1, last_1, first_2, last_2, result, comp);
  }
  else{
    return std::merge( first_1, last_1, first_2, last_2, result, comp);
  }
}
} // namespace omp_merge

} // namespace omp_algorithm

#endif