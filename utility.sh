# File: utility.sh
# By: DrkWithT
# Brief: wraps CMake & CTest invocations, even other useful cmds.

if [[ $# -ne 1 ]]; then
    echo "Usage: ./utility.sh [info | debug | release | test | sloc]"
    exit 1
fi

option="$1"

if [[ "$option" = "debug" ]]; then
    cmake --fresh -S . -B build --preset vsc-gcc-debug-config && cmake --build build;
    cp ./build/compile_commands.json .
elif [[ "$option" = "release" ]]; then
    cmake --fresh -S . -B build --preset vsc-gcc-release-config && cmake --build build;
    cp ./build/compile_commands.json .
elif [[ "$option" = "info" ]]; then
    echo "Usage: ./utility.sh [info | debug | release | test]"
elif [[ "$option" = "test" ]]; then
    ctest --test-dir build --timeout 3
elif [[ "$option" = "sloc" ]]; then
    wc -l ./src/**/*.cpp ./src/include/**/*.hpp
else
    echo "Invalid option!"
    exit 1
fi
