cmake_minimum_required(VERSION 3.28)

project(CMakePresetDemo VERSION 0.0.1 LANGUAGES CXX)

# default var. settings
# (use preset)

if (DO_DEBUG_BUILD)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -Og)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -O2)
endif()

enable_testing()
add_subdirectory(src)
