cmake_minimum_required (VERSION 3.6)
project (zmsgr)

enable_testing ()

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-g -Wall -Werror")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

FILE (GLOB SOURCES src/*.cpp)
add_library (zmsgr SHARED ${SOURCES})
target_link_libraries (zmsgr
    zmq
    pthread
)
target_include_directories(zmsgr PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable (reqrep tests/reqrep.cpp)
target_link_libraries (reqrep zmsgr)
add_test (NAME reqrep COMMAND reqrep)

add_executable (mtserver tests/mtserver.cpp)
target_link_libraries (mtserver zmsgr)
add_test (NAME mtserver COMMAND mtserver)

add_executable (lpclient tests/lpclient.cpp)
target_link_libraries (lpclient zmsgr)
add_test (NAME lpclient COMMAND lpclient)
