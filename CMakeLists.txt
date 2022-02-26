cmake_minimum_required(VERSION 3.21)

project(naobi)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -pedantic -Werror)

include(FetchContent)

set(NAOBI_SOURCES
        source/compiler/compiler.cpp
        source/utils/parser.cpp
        source/data/module.cpp
        source/data/function.cpp
        source/data/variable.cpp
        source/data/workflow.cpp)

set(NAOBI_TEST_SOURCES
        test/module/parser_test.cpp)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG main
        SOURCE_DIR library
)
FetchContent_MakeAvailable(googletest)

find_package(GTest REQUIRED)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${NAOBI_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(${CMAKE_PROJECT_NAME} source/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)

add_executable(${CMAKE_PROJECT_NAME}_test ${NAOBI_TEST_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC ${GTEST_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME}_test gtest gmock gtest_main pthread ${CMAKE_PROJECT_NAME}_lib)
