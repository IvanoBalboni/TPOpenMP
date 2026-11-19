auto co_rank = [&](std::size_t i){
    std::size_t j = std::min(i, m);
    std::size_t k = i - j;                        // j + k == i
    std::size_t k_low = 0;
    std::size_t j_low = (i > n) ? i - n : 0;      // max(0, i - n) 
    std::bool active = true
    while (active) {
        if (j > 0 && k < n && A[j - 1] > B[k]){
            const std::size_t delta = (j - j_low + 1) / 2;   /
            k_low = k;
            j -= delta; k += delta;
        } 
        else if (k > 0 && j < m && B[k - 1] >= A[j]){
            const std::size_t delta =(k - k_low + 1) / 2;   
            j_low = j;
            j += delta;
            k -= delta;
        }
        else {
            active= false
        };
    }
    return std::pair<std::size_t, std::size_t>{j, k};
};