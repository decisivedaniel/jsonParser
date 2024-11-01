cmake_minimum_required(VERSION 3.14)
project(json_eval)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    hello_test
    hello_test.cc
)

add_executable(
    json_eval
    json_eval.cpp
)

add_executable(
    json_eval_unittest
    json_eval_unittest.cc
)

add_library(json json)

target_link_libraries(
    json_eval_unittest
    GTest::gtest_main
)

target_link_libraries(
    hello_test
    GTest::gtest_main
)

target_link_libraries(
    json_eval_unittest
    json
)

target_link_libraries(
    json_eval
    json
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(json_eval_unittest)

add_custom_command(TARGET json_eval PRE_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/test/ $<TARGET_FILE_DIR:json_eval>)           