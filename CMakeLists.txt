cmake_minimum_required(VERSION 3.10)

project(lab1OS CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Тип сборки" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g" CACHE STRING "Debug flags")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops -g" CACHE STRING "Release flags")

file(GLOB SHELL_SOURCES
        "src/Shell.cpp"
        "src/Shell.h"
        "src/Commands/*.cpp"
        "src/Commands/*.h"
)

# Define the source files for the benchmarks
file(GLOB BENCH_SOURCES_FACTORIZE
        "src/Bench/FactorizeBenchmark.cpp"
        "src/Bench/FactorizeBenchmark.h"
)

file(GLOB BENCH_SOURCES_EMASEARCH
        "src/Bench/EmaSearchStrBenchmark.cpp"
        "src/Bench/EmaSearchStrBenchmark.h"
)

# Add executable files
add_executable(Main src/Main.cpp ${SHELL_SOURCES})

add_executable(factorize src/Bench/factorize.cpp ${BENCH_SOURCES_FACTORIZE})
add_executable(emasearch src/Bench/emasearch.cpp ${BENCH_SOURCES_EMASEARCH})
add_executable(combined src/Bench/combined.cpp ${BENCH_SOURCES_FACTORIZE} ${BENCH_SOURCES_EMASEARCH})
