cmake_minimum_required(VERSION 3.13)
project(CodingKatas)

file(GLOB CLANG_FORMAT_BIN_NAME /usr/bin/clang-format)
if(NOT CLANG_FORMAT_BIN_NAME-NOTFOUND)
    message(STATUS "Code Formatter:  ${CLANG_FORMAT_BIN_NAME}")
    file(GLOB_RECURSE ALL_SOURCE_FILES *.h *.cc)
    add_custom_target(
            format
            COMMAND ${CLANG_FORMAT_BIN_NAME}
            -i
            ${ALL_SOURCE_FILES}
    )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
enable_testing()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(BloomFilter)
add_subdirectory(Bowling)
add_subdirectory(LeapYear)
add_subdirectory(Tennis)