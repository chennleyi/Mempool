cmake_minimum_required(VERSION 3.0)
project(memorypool)
add_compile_options(-g)
include_directories(src)
file(GLOB FILES src/*)
add_executable(
  mempool
  ./main.cpp
  ${FILES}
)
target_link_libraries(mempool)



