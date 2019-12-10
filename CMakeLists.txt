CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

SET(CMAKE_CXX_STANDARD 11)
SET(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/unit_tests")
SET(PROJECT_BINARY_DIR "${PROJECT_BINARY_DIR}/src")

ADD_EXECUTABLE(test
    ${PROJECT_SOURCE_DIR}/unit_test.cpp
    ${PROJECT_BINARY_DIR}/parser.cpp
    ${PROJECT_BINARY_DIR}/execute.cpp
    ${PROJECT_BINARY_DIR}/testexecute.cpp
    ${PROJECT_BINARY_DIR}/and.cpp
    ${PROJECT_BINARY_DIR}/or.cpp
    ${PROJECT_BINARY_DIR}/semi.cpp
    ${PROJECT_BINARY_DIR}/exit.cpp
    ${PROJECT_BINARY_DIR}/factory.cpp
    ${PROJECT_BINARY_DIR}/outputredirect.cpp
    ${PROJECT_BINARY_DIR}/inputredirect.cpp
)

ADD_EXECUTABLE(rshell
    ${PROJECT_BINARY_DIR}/rshell.cpp
    ${PROJECT_BINARY_DIR}/parser.cpp
    ${PROJECT_BINARY_DIR}/execute.cpp
    ${PROJECT_BINARY_DIR}/testexecute.cpp
    ${PROJECT_BINARY_DIR}/and.cpp
    ${PROJECT_BINARY_DIR}/or.cpp
    ${PROJECT_BINARY_DIR}/semi.cpp
    ${PROJECT_BINARY_DIR}/exit.cpp
    ${PROJECT_BINARY_DIR}/factory.cpp
)

TARGET_LINK_LIBRARIES(test gtest)
TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)


