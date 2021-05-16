cmake_minimum_required( VERSION 3.10 )
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
set( THREADS_PREFER_PTHREAD_FLAG TRUE )

project( timer VERSION 1.0 )

find_package( Threads REQUIRED )
find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIR} )

add_library( Clock STATIC Clock/clock.cpp )
target_include_directories( Clock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Clock/include )

add_executable( ${PROJECT_NAME} main.cpp )
target_link_libraries( ${PROJECT_NAME} PUBLIC Clock )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${CURSES_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT} )