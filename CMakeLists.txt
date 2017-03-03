cmake_minimum_required (VERSION 2.6)

if ( UNIX )
enable_testing()
endif ()

set ( CMAKE_C_COMPILER gcc )
set ( CMAKE_CXX_COMPILER g++ )
set ( CMAKE_BUILD_TYPE Debug )

add_definitions( "-DHAVE_STRUCT_TIMESPEC -O0 -g3" )

project ( QUEUE )

add_subdirectory ( src )