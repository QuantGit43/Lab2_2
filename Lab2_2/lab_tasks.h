#pragma once

#include <vector>
#include <cstdint>    
#include <limits>     
#include <numeric>    
#include <algorithm>  
#include <execution>  

using DataType = int64_t;
constexpr DataType DATA_MIN = -1'000'000'000;
constexpr DataType DATA_MAX = 1'000'000'000;
constexpr DataType MIN_DIFF = std::numeric_limits<DataType>::min();


std::vector<DataType> create_random_data(size_t size);

// Завдання 1
DataType find_max_diff_serial(const std::vector<DataType>& data);

// Завдання 3
DataType find_max_diff_custom(const std::vector<DataType>& data, unsigned int K);


// Завдання 2
template<typename Policy>
DataType find_max_diff_policy(const std::vector<DataType>& data, Policy policy)
{
    if (data.size() < 2) return 0;
    std::vector<DataType> diffs(data.size());

    std::adjacent_difference(policy, data.begin(), data.end(), diffs.begin());

    return std::reduce(policy,
        diffs.begin() + 1,
        diffs.end(),
        MIN_DIFF,
        [](DataType a, DataType b) { return std::max(a, b); }
    );
}