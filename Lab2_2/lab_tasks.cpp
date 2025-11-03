#include "lab_tasks.h" // Включаємо наш власний заголовковий файл

#include <random>       // Для std::mt19937
#include <algorithm>    // Для std::generate, std::max_element
#include <numeric>      // Для std::adjacent_difference
#include <thread>       //
#include <latch>        //
#include <cmath>        // Для std::min

/**
 * @brief Створює вектор випадкових даних.
 */
std::vector<DataType> create_random_data(size_t size)
{
    std::vector<DataType> data(size);
    std::mt19937 gen(12345);
    std::uniform_int_distribution<DataType> dist(DATA_MIN, DATA_MAX);
    std::generate(data.begin(), data.end(), [&]() { return dist(gen); });
    return data;
}

// ----------------------------------------------------------------------------
// ЗАВДАННЯ 1: Реалізація
// ----------------------------------------------------------------------------
DataType find_max_diff_serial(const std::vector<DataType>& data)
{
    if (data.size() < 2) return 0;
    std::vector<DataType> diffs(data.size());
    std::adjacent_difference(data.begin(), data.end(), diffs.begin());
    auto max_it = std::max_element(diffs.begin() + 1, diffs.end());
    return (max_it == diffs.end()) ? MIN_DIFF : *max_it;
}

// ----------------------------------------------------------------------------
// ЗАВДАННЯ 3: Реалізація
// ----------------------------------------------------------------------------
DataType find_max_diff_custom(const std::vector<DataType>& data, unsigned int K)
{
    if (data.size() < 2) return 0;
    if (K == 0) K = 1;

    std::vector<DataType> local_maxes(K, MIN_DIFF);
    size_t chunk_size = (data.size() + K - 1) / K;

    // Використовуємо std::latch, як показано у latches.cpp
    std::latch work_done(K);

    std::vector<std::thread> threads; //
    threads.reserve(K);

    for (unsigned int i = 0; i < K; ++i)
    {
        threads.emplace_back([&data, &local_maxes, &work_done, i, K, chunk_size] {

            size_t start_idx = i * chunk_size;
            size_t end_idx = std::min(data.size(), (i + 1) * chunk_size);
            size_t op_start_idx = (start_idx == 0) ? 0 : start_idx - 1;

            if (end_idx <= op_start_idx + 1)
            {
                work_done.count_down(); //
                return;
            }

            std::vector<DataType> local_diffs(end_idx - op_start_idx);

            std::adjacent_difference(std::execution::seq,
                data.begin() + op_start_idx,
                data.begin() + end_idx,
                local_diffs.begin());

            size_t diff_start_in_local = 1;

            if (diff_start_in_local < local_diffs.size())
            {
                local_maxes[i] = *std::max_element(local_diffs.begin() + diff_start_in_local,
                    local_diffs.end());
            }

            work_done.count_down(); //
            });
    }

    work_done.wait(); //

    for (auto& t : threads)
    {
        t.join();
    }

    return *std::max_element(local_maxes.begin(), local_maxes.end());
}