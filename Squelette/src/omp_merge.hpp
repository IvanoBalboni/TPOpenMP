#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <omp.h>
#include <type_traits>
#include <utility>



namespace omp_algorithm::omp_merge {
    template <class It1, class It2, class OutIt, class Comp>
    OutIt omp_array_impl(It1 A, It1 A_end, It2 B, It2 B_end, OutIt C, Comp comp) {
        const std::size_t m = std::distance(A, A_end);
        const std::size_t n = std::distance(B, B_end);
        const std::size_t total = m + n;


    auto co_rank = [&](std::size_t i){
        std::size_t j = std::min(i, m);
        std::size_t k = i - j;                        //j+k==i
        std::size_t k_low = 0;
        std::size_t j_low = (i > n) ? i - n : 0;      //max(0,i-n) 
        bool active = true;
        while (active) {
            if (j > 0 && k < n && comp(B[k], A[j-1])){
                const std::size_t delta = (j - j_low + 1)/2;   /
                k_low = k;
                j -= delta; k += delta;
            } 
            else if (k > 0 && j < m && !comp(B[k-1], A[j])){
                const std::size_t delta =(k - k_low + 1)/2;   
                j_low = j;
                j += delta;
                k -= delta;
            }
            else {
                active= false;
            };
        }
        return std::pair<std::size_t, std::size_t>{j, k};
    };
    (void)co_rank;
    return std::merge(A, A_end, B, B_end, C, comp);
    }
}



