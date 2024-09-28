cmake_minimum_required(VERSION 3.10)
project(LLVMLang)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lm")

find_package(ZLIB REQUIRED)

set(LLVM_DIR, $(llvm-config --prefix))
find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(wlang main.cpp)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(wlang ${llvm_libs})

