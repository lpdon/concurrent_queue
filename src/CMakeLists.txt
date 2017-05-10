#SRC

include_directories ( 
  ${PROJECT_SOURCE_DIR}/include/  
  ${PROJECT_SOURCE_DIR}/lib/googletest/googletest/include 
)

file (
  GLOB_RECURSE
  queue_headers
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
)

file (
  GLOB_RECURSE
  queue_src
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

find_package (Threads REQUIRED)

add_executable ( 
  test_queue
  ${queue_headers}
  ${queue_src}
) 

target_compile_features( 
  test_queue PRIVATE 
  cxx_auto_type 
  cxx_nullptr 
  cxx_lambdas 
)

target_link_libraries ( 
  test_queue 
  ${CMAKE_THREAD_LIBS_INIT}
 )