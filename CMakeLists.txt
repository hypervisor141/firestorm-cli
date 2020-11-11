cmake_minimum_required(VERSION 3.17.0)
project(firestorm)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS, "-static -g -std=c++17 -Wall -Wextra -Werror -arch x86_64 -mmitigate-rop -fstack-protector-all 
    -Wstack-protector --param ssp-buffer-buffer-size=4 -pie -fPIE -ftrapv -D_FORTIFY_SOURCE=2 -O2 -Wl,-z,reloz,now")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.cpp)

add_executable(firestorm ${SOURCES} ${HEADERS})

set_target_properties(firestorm
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
