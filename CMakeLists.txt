cmake_minimum_required(VERSION 3.15)
project(otuscpp_hw_1_ipfilter)

set(CMAKE_CXX_STANDARD 20)

set(DEFAULT_TARGET_NAME ipFilter)

if("${TARGET_NAME}" STREQUAL "")
    # this branch will be taken
    message(STATUS "Cmake run with empty target name. Defaulting it to ${DEFAULT_TARGET_NAME}")
    set(TARGET_NAME ${DEFAULT_TARGET_NAME})
else()
    message(STATUS "Target name is: ${TARGET_NAME}")
endif()

#using UTC time by default. Making default CMake behaviour explicit.
string(TIMESTAMP TODAY "%Y-%m-%dT%H:%M:%S")

message(STATUS "Build time is: ${TODAY}")

file(READ "Versioning.txt" version)

string(REGEX MATCH "HOMEWORK_MAJOR_NUMBER ([0-9]*)" _ ${version})
set(HOMEWORK_MAJOR_NUMBER ${CMAKE_MATCH_1})

string(REGEX MATCH "HOMEWORK_MINOR_NUMBER ([0-9]*)" _ ${version})
set(HOMEWORK_MINOR_NUMBER ${CMAKE_MATCH_1})

set(HOMEWORK_MINOR_NUMBER ${CMAKE_MATCH_1})

if (BUILD_NUMBER)
    set(HOMEWORK_BUILD_NUMBER ${BUILD_NUMBER})
else()
    string(REGEX MATCH "HOMEWORK_BUILD_NUMBER ([0-9]*)" _ ${version})
    set(HOMEWORK_BUILD_NUMBER ${CMAKE_MATCH_1})
endif()

#Obviously, this is crutch. I don't know hot to make this more beautifull.
#It is necessary to pass this environment variable to the post-build bash script.
#But it is up to Cmake to define this value on the run...
#What to do???
file(WRITE binary_version.txt "${HOMEWORK_MAJOR_NUMBER}.${HOMEWORK_MINOR_NUMBER}.${HOMEWORK_BUILD_NUMBER}")

message(STATUS "Build Number: ${HOMEWORK_BUILD_NUMBER}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/version.cpp)

add_executable(${TARGET_NAME} main.cpp version.cpp version.h BuildNumber.h)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR} )
