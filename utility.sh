# File: utility.sh
# Brief: wraps CMake invocations

if [[ $# -ne 1 ]]; then
    echo "Usage: ./utility.sh [info | debug | release]"
    exit 1
fi

option="$1"

if [[ "$option" = "debug" ]]; then
    cmake --fresh -S . -B build --preset vsc-gcc-debug-config && cmake --build build && cp ./build/compile_commands.json .
elif [[ "$option" = "release" ]]; then
    cmake --fresh -S . -B build --preset vsc-gcc-release-config && cmake --build build && cp ./build/compile_commands.json .
elif [[ "$option" = "info" ]]; then
    echo "Usage: ./utility.sh [info | debug | release]"
else
    echo "Invalid option!"
    exit 1
fi
