cmake_minimum_required(VERSION 3.21)

project(mlogger CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(mlogger INTERFACE
        include/Logger.h
        include/LoggerImpl.h
        include/Severity.h
        src/Utils.h
        )

target_include_directories(mlogger INTERFACE include)
target_include_directories(mlogger INTERFACE pch)

target_link_libraries(mlogger INTERFACE ${CONAN_LIBS_BOOST})

target_precompile_headers(mlogger INTERFACE pch/pch.h)