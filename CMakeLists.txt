cmake_minimum_required (VERSION 2.6)

if ( UNIX )
enable_testing()
endif ()

set ( CMAKE_C_COMPILER gcc )
set ( CMAKE_CXX_COMPILER g++ )
set ( CMAKE_BUILD_TYPE Debug )
set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )

project ( queue C CXX )

add_subdirectory ( src )  

if ( UNIX )
add_subdirectory ( ${QUEUE_SOURCE_DIR}/lib/googletest/googletest/ )
add_subdirectory ( test )
add_test (NAME ut COMMAND run_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
endif ()