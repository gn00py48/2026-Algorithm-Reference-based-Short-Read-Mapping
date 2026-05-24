cmake_minimum_required(VERSION 3.16)
project(short_read_mapping LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(short_read_mapping
    src/main.cpp
    src/common.cpp
    src/algorithms/native_sliding.cpp
    src/algorithms/kmp_seed.cpp
    src/algorithms/hash_seed.cpp
    src/algorithms/weighted_hash_seed.cpp
)
