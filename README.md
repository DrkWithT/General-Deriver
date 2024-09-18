# CMake Preset Demo

### Brief:
My first, personal template for C++ projects using CMake Presets. Requires VSCode, clangd with the dedicated extension, and CMake 3.28+.

### Notes:
 - *nix or WSL setups with GCC installed will likely work.
 - For 1st build: `cmake -S . -B build --preset vsc-gcc-debug-config && cmake --build build && cp ./build/compile_commands.json .`
    - Add the `--fresh` option to CMake on successive builds to reset variables.
