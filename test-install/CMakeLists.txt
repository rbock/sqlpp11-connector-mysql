cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(test-install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(sqlpp-mysql REQUIRED)

add_executable(test-install main.cpp)

target_link_libraries(test-install PRIVATE sqlpp11::mysql)
