cmake_minimum_required(VERSION 3.20)
project(PSD_Final_Project)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(PSD_Final_Project ${SOURCES})

target_include_directories(
    PSD_Final_Project
    PRIVATE
    src
    include
)

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})