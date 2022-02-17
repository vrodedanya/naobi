cmake_minimum_required(VERSION 3.21)

project(naobi)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(NAOBI_TEST_SOURCES test/test.cpp)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG main
        SOURCE_DIR library
)
FetchContent_MakeAvailable(googletest)

find_package(GTest REQUIRED)


add_executable(naobi_test ${NAOBI_TEST_SOURCES})
target_include_directories(naobi_test PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(naobi_test gtest gtest_main pthread)
