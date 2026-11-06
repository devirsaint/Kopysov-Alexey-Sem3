cmake_minimum_required(VERSION 3.20)
project(MatrixSolution LANGUAGES CXX)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_library(core STATIC
    include/Matrix.h src/Matrix.cpp
    include/Generator.h src/Generator.cpp
    include/ConstantGenerator.h src/ConstantGenerator.cpp
    include/ZeroGenerator.h src/ZeroGenerator.cpp
    include/RandomGenerator.h src/RandomGenerator.cpp
    include/Exercise.h src/Exercise.cpp
    include/Task1.h src/Task1.cpp
    include/Task2.h src/Task2.cpp
)
target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(main
    src/main.cpp
    include/IStreamGenerator.h src/IStreamGenerator.cpp
)
target_link_libraries(main PRIVATE core)

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(googletest
    URL https://codeload.github.com/google/googletest/zip/refs/tags/v1.17.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests
    test/test.cpp
    include/IStreamGenerator.h src/IStreamGenerator.cpp
)
target_link_libraries(tests PRIVATE core GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

foreach(target core main tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8 /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT main)
