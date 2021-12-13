cmake_minimum_required(VERSION 3.21)

project(analytics CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(analytics main.cpp)

target_link_libraries(analytics
        mlogger
        service
        context
)