#SRC

include_directories ( 
  ${QUEUE_SOURCE_DIR}/include
)

find_package (Threads REQUIRED)

add_executable ( 
  test_queue
  main.cpp
) 

set (
  queue_src_lib
  queue.cpp
  list.cpp
)

add_library (
  queue_lib
  ${queue_src_lib}
  ${queue_include_lib}
)

target_link_libraries (
  queue_lib
  ${CMAKE_THREAD_LIBS_INIT}
) 

target_link_libraries ( 
  test_queue 
  queue_lib
 )