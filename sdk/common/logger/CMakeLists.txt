cmake_minimum_required(VERSION 3.21)

project(mlogger CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(mlogger INTERFACE)
target_sources(mlogger INTERFACE
        Logger.h
        LoggerImpl.h
        Severity.h
        Utils.h
)

target_link_libraries(mlogger INTERFACE ${CONAN_LIBS_BOOST})

#target_precompile_headers(mlogger INTERFACE pch/pch.h)