cmake_minimum_required(VERSION 3.10)
project(polymorphism_bench)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")

add_subdirectory(scoped_timer)

add_executable(bench main.cpp task_ref.hpp)
target_compile_features(bench PRIVATE cxx_std_17)
target_link_libraries(bench CONAN_PKG::boost CONAN_PKG::fmt scoped_timer::scoped_timer)





