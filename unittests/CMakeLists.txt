
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

function(add_unit_test name)
  add_executable(${name} "${name}.cpp")
  target_link_libraries(${name} ${GTEST_BOTH_LIBRARIES})
  target_link_libraries(${name} ${CMAKE_THREAD_LIBS_INIT})
  add_test(${name} ${name})
endfunction()

add_unit_test(SlabPoolTest)
