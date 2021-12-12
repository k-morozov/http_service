cmake_minimum_required(VERSION 3.21)

project(service CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(service STATIC)

target_include_directories(service PUBLIC include)
target_include_directories(service PRIVATE src)

target_sources(service PUBLIC
    include/IService.h
    include/Gateway.h
        src/gateway/Gateway.cpp
)

target_link_libraries(service
        PUBLIC ${CONAN_LIBS_BOOST}
        mlogger
)

#target_precompile_headers(mlogger INTERFACE pch/pch.h)