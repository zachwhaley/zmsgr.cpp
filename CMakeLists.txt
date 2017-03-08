cmake_minimum_required (VERSION 3.6)
project (zmsgr)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-g -Wall -Werror")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

FILE (GLOB SOURCES *.cpp)
add_library (zmsgr SHARED ${SOURCES})
target_link_libraries (zmsgr
    zmq
    pthread
)

add_executable (reqrep EXCLUDE_FROM_ALL samples/reqrep.cpp)
add_executable (mtserver EXCLUDE_FROM_ALL samples/mtserver.cpp)
add_executable (lpclient EXCLUDE_FROM_ALL samples/lpclient.cpp)
target_link_libraries (reqrep zmsgr)
target_link_libraries (mtserver zmsgr)
target_link_libraries (lpclient zmsgr)
target_include_directories (reqrep PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories (mtserver PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories (lpclient PRIVATE ${CMAKE_SOURCE_DIR})

add_custom_target (samples
    DEPENDS reqrep mtserver lpclient
)
