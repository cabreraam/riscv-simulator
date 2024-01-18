cmake_minimum_required(VERSION 3.12)
project(riscv-simulator)

set(CMAKE_CXX_STANDARD 14)
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()
set(CMAKE_CXX_FLAGS -v ${CMAKE_CXX_FLAGS})

#include pthreads
find_package(Threads)

# include the google test
#include(GoogleTest)
find_package(GTest REQUIRED)

message("${GTest_INCLUDE_DIRS}")

# create the main app
set(SOURCE_FILES cpu/alu.cpp cpu/alu.h cpu/reg.cpp cpu/reg.h cpu/cpu.cpp cpu/cpu.h mem/mem.cpp mem/mem.h translator/translator.h translator/translator.cpp)
add_executable(risc-simulator main.cpp ${SOURCE_FILES})

# create the translator app
add_executable(risc-translator translator/trans_main.cpp translator/translator.h translator/translator.cpp)
target_include_directories(risc-translator SYSTEM PUBLIC /usr/local/include)
target_link_directories(risc-translator PUBLIC /usr/local/lib)

# create the alu test
add_executable(TestAll test/test-alu.cpp ${SOURCE_FILES} test/test-cpu.cpp)
target_link_libraries(TestAll gtest_main gtest ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(TestAll PUBLIC "${PROJECT_SOURCE_DIR}/test")# /usr/local/include)
gtest_add_tests(TARGET TestAll)
target_include_directories(TestAll SYSTEM PUBLIC /usr/local/include)
target_link_directories(TestAll PUBLIC /usr/local/lib)
