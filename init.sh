conan install . --build=missing -s build_type=Debug
cmake --preset conan-debug
cmake -B build/Debug --preset conan-debug
