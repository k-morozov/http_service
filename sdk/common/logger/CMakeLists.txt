cmake_minimum_required(VERSION 3.21)

project(mlogger CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
        Logger.h
        LoggerBase.h
        Severity.h
        Utils.h
)

target_link_libraries(${PROJECT_NAME} INTERFACE ${CONAN_LIBS_BOOST})

#target_precompile_headers(mlogger INTERFACE pch/pch.h)