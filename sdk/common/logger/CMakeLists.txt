cmake_minimum_required(VERSION 3.21)

project(mlogger)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(pch)

add_library(mlogger STATIC src/logger.cpp src/LoggerImpl.cpp)

target_link_libraries(mlogger ${CONAN_LIBS_BOOST})

target_precompile_headers(mlogger PRIVATE pch/pch.h)