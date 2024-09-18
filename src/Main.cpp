/**
 * @file Main.cpp
 * @author DrkWithT
 * @brief Implements test program.
 * @version 0.0.1
 * @date 2024-09-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <format>
#include <iostream>
#include <vector>

#include "Foo/Foo.hpp"

int main() {
    using FooLib::sumAll;

    std::vector<int> test_nums = {1, 2, 3, 4};

    std::cout << std::format("sum is {}\n", sumAll(test_nums));
}