#SRC

include_directories ( 
  ${QUEUE_SOURCE_DIR}/include  
  ${QUEUE_SOURCE_DIR}/lib/googletest/googletest/include 
)

find_package (Threads REQUIRED)

add_executable ( 
  test_queue
  main.cpp
) 

target_link_libraries ( 
  test_queue 
  ${CMAKE_THREAD_LIBS_INIT}
 )