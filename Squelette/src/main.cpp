#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <omp.h>

#include "omp_algorithm.hpp"

// Container content.
using Elt_Type = int;

// Parameters for full array version.
#if defined(OMP_MERGE_ARRAY)
#include <vector>
const std::string NAME = "array version";
constexpr std::size_t LOOP_ITER_NUM = 40'000;
using Src_Container = std::vector<Elt_Type>;
using Dest_Container = std::vector<Elt_Type>;

// Parameters for non array version.
#elif defined(OMP_MERGE_NOT_ARRAY)
#include <list>
const std::string NAME = "not array version";
constexpr std::size_t LOOP_ITER_NUM = 1;
using Src_Container = std::list<Elt_Type>;
using Dest_Container = std::list<Elt_Type>;
#endif

// Source container sizes.
constexpr std::size_t RANGE_1_SIZE = (2 * 64) * 1024;
constexpr std::size_t RANGE_2_SIZE = RANGE_1_SIZE + 211;

/**
 * @brief Compares results of sequential and parallel version of the algorithm.
 *
 * @return @c true if results match else @c false.
 */
bool test_merge_func(void) noexcept;

/**
 * @brief Prints speedup and efficiency of the parallel version of the
 * algorithm.
 *
 */
void run_merge_func(void) noexcept;

/**
 * @brief Main program.
 *
 * @return @c EXIT_SUCCESS.
 */
int main(void) {

  // Test then run this implementation.
  if (not test_merge_func()) {
    std::cout << "failed" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "passed" << std::endl;
  run_merge_func();

  // It's over.
  return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/*                               test_merge_func                              */
/* -------------------------------------------------------------------------- */

bool test_merge_func(void) noexcept {

  // Source containers.
  Src_Container src_1(RANGE_1_SIZE), src_2(RANGE_2_SIZE);
  std::iota(src_1.begin(), src_1.end(), 19);
  std::iota(src_2.begin(), src_2.end(), 5);

  // Comparator.
  constexpr std::less<Elt_Type> comp;

  // Expected result.
  Dest_Container result(src_1.size() + src_2.size());
  std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
             result.begin(), comp);

  // Destination array.
  Dest_Container dest(result.size());

  // Let's go.
  omp_algorithm::omp_merge::merge(src_1.begin(), src_1.end(), src_2.begin(),
                                  src_2.end(), dest.begin(), comp);

  // Compare the two result arrays.
  const auto last = result.cend();
  const auto res = std::mismatch(result.cbegin(), last, dest.cbegin());
  return (res.first == last);
}

/* -------------------------------------------------------------------------- */
/*                               run_merge_func                               */
/* -------------------------------------------------------------------------- */

void run_merge_func(void) noexcept {

  int thread_num;
#pragma omp parallel
#pragma omp single
  thread_num = omp_get_num_threads();

  // Comparator.
  constexpr std::less<Elt_Type> comp;

  // Source containers.
  Src_Container src_1(RANGE_1_SIZE), src_2(RANGE_2_SIZE);
  std::iota(src_1.begin(), src_1.end(), 19);
  std::iota(src_2.begin(), src_2.end(), 5);

  // Destination array.
  Dest_Container result(src_1.size() + src_2.size());

  // Sequential version.
  double seq_time;
  {
    const double start = omp_get_wtime();
    for (std::size_t i = 0; i < LOOP_ITER_NUM; i++) {
      std::merge(src_1.begin(), src_1.end(), src_2.begin(), src_2.end(),
                 result.begin(), comp);
    }
    const double stop = omp_get_wtime();
    seq_time = stop - start;
  }

  // Parallel version.
  double par_time;
  {
    const double start = omp_get_wtime();
    for (std::size_t i = 0; i < LOOP_ITER_NUM; i++) {
      omp_algorithm::omp_merge::merge(src_1.begin(), src_1.end(), src_2.begin(),
                                      src_2.end(), result.begin(), comp);
    }
    const double stop = omp_get_wtime();
    par_time = stop - start;
  }

  // Print results.
  std::cout << "--[ omp_merge, " << NAME << ": begin ] --" << std::endl;
  std::cout << "\tSeq. time :\t\t" << seq_time << " sec." << std::endl;
  std::cout << "\tPar. time :\t\t" << par_time << " sec." << std::endl;
  std::cout << "\tThread(s):\t\t" << thread_num << std::endl;
  std::cout << "\tSpeedup:\t\t" << seq_time / par_time << std::endl;
  std::cout << "\tEfficiency:\t\t" << (seq_time / par_time) / thread_num
            << std::endl;
  std::cout << "--[ omp_merge, " << NAME << ": end ] --" << std::endl;
  std::cout << std::endl;
}