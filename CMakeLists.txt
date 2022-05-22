cmake_minimum_required(VERSION 3.21)

project(naobi VERSION 0.6.2)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(NAOBI_SOURCES
        source/compiler/compiler.cpp
        source/compiler/code_generator.cpp
        source/data/module.cpp
        source/data/function.cpp
        source/data/variable.cpp
        source/data/workflow.cpp
        source/interpreter/handler.cpp
        source/interpreter/event_manager.cpp
        source/standard/standard_module.cpp
        source/utils/type.cpp
        source/utils/arguments.cpp
        source/utils/parser.cpp
        )

set(NAOBI_TEST_SOURCES
        test/module/parser_test.cpp
        test/module/type_test.cpp
        )

find_package(GTest REQUIRED)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${NAOBI_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(${CMAKE_PROJECT_NAME}_lib PUBLIC -pedantic -Wall -Wextra -Wcast-align -Wcast-qual
        -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations
        -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow
        -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror
        -Wno-unused)

add_executable(${CMAKE_PROJECT_NAME} source/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}
        VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}
        VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}
        )

add_executable(${CMAKE_PROJECT_NAME}_test ${NAOBI_TEST_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC ${GTEST_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME}_test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${CMAKE_PROJECT_NAME}_test gtest gmock gtest_main pthread ${CMAKE_PROJECT_NAME}_lib)
