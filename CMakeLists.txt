cmake_minimum_required(VERSION 2.6.4)

project(SlabPool)

enable_testing()

include_directories(.)
add_subdirectory(unittests)

install(FILES SlabPool.h DESTINATION include) 
