/**
 * @file Foo.cpp
 * @author DrkWithT
 * @brief Implements sample, placeholder library.
 * @version 0.0.1
 * @date 2024-09-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <numeric>
#include "Foo/Foo.hpp"

namespace FooLib {
    [[nodiscard]] int sumAll(const std::vector<int>& nums) {
        return std::accumulate(nums.begin(), nums.end(), 0);
    }
}