#include <iostream>
#include <iomanip>     
#include <vector>
#include <thread>      
#include <execution>   
#include <functional>  

#include "timing.h"   
#include "lab_tasks.h"

//Compiler : C++23

int main()
{
    system("chcp 65001");
    std::cout << "========================================================\n";
    std::cout << "Optimization: [Max optimization]\n";

    const unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Hardware concurrency (CPU threads): " << num_cores << "\n";
    std::cout << "========================================================\n\n";

    std::vector<size_t> sizes = { 100'000, 1'000'000, 20'000'000 };

    for (const size_t size : sizes)
    {
        std::cout << "--- Testing with data size: " << size << " ---\n";
        auto data = create_random_data(size);
        DataType serial_res, par_res_seq, par_res_par, par_res_par_unseq;

        std::cout << std::left << std::setw(35) << "1. Serial (no policy):";
        timeit([&]() { 
            serial_res = find_max_diff_serial(data);
            });

        std::cout << std::left << std::setw(35) << "2a. Policy (seq):";
        timeit([&]() { 
            par_res_seq = find_max_diff_policy(data, std::execution::seq);
            });

        std::cout << std::left << std::setw(35) << "2b. Policy (par):";
        timeit([&]() { 
            par_res_par = find_max_diff_policy(data, std::execution::par);
            });

        std::cout << std::left << std::setw(35) << "2c. Policy (par_unseq):";
        timeit([&]() { 
            par_res_par_unseq = find_max_diff_policy(data, std::execution::par_unseq);
            });

        std::cout << "Results: "
            << serial_res << ", " << par_res_seq << ", "
            << par_res_par << ", " << par_res_par_unseq << "\n\n";


        std::cout << "--- Task 3: Custom K-Threads ---\n";
        std::cout << std::left << std::setw(8) << "K"
            << std::setw(15) << "Time (us)"
            << "Max Diff\n";
        std::cout << std::string(40, '-') << "\n";

        long long best_time_us = -1;
        unsigned int best_k = 0;

        std::vector<unsigned int> K_values;
        for (unsigned int k = 1; k <= num_cores * 2; ++k)
        {
            K_values.push_back(k);
        }
        K_values.push_back(num_cores * 4);
        K_values.push_back(num_cores * 8);

        for (const unsigned int K : K_values)
        {
            auto start = std::chrono::high_resolution_clock::now();
            DataType max_diff = find_max_diff_custom(data, K);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            std::cout << std::left << std::setw(8) << K
                << std::setw(15) << duration_us
                << max_diff << "\n";

            if (best_time_us == -1 || duration_us < best_time_us)
            {
                best_time_us = duration_us;
                best_k = K;
            }
        }

        std::cout << "\nBest performance found:\n";
        std::cout << "  Best K: " << best_k << "\n";
        std::cout << "  Time:   " << best_time_us << " us\n";
        std::cout << "  Ratio (Best K / CPU Cores): "
            << std::fixed << std::setprecision(2)
            << (static_cast<double>(best_k) / num_cores) << "\n";
        std::cout << "------------------------------------------\n\n";
    }

    return 0;
}