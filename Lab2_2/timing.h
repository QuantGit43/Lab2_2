#pragma once

#include <iostream>
#include <chrono>
#include <format>
#include <functional>

template <class F>
void timeit(F&& f)
{
    using namespace std;
    auto time1 = std::chrono::system_clock::now();
    f();
    auto time2 = std::chrono::system_clock::now();
    auto tdiff = time2 - time1;
    if (tdiff > 100000ns)
        std::cout << std::format("{:%S}", tdiff);
    else
        std::cout << tdiff;
    std::cout << std::endl;
}