#TEST

include_directories (
  ${QUEUE_SOURCE_DIR}/include
  ${QUEUE_SOURCE_DIR}/lib/googletest/googletest/include 
)

set (CMAKE_BUILD_TYPE Debug)
add_definitions( -Wall )

set (
  queue_test
  test.cpp
)

add_executable (run_test ${queue_test})
target_link_libraries (run_test queue_lib gtest gtest_main )