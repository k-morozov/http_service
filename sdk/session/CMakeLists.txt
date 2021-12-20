cmake_minimum_required(VERSION 3.21)

project(session CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

target_sources(${PROJECT_NAME} PRIVATE
        include/ISession.h
        include/SessionBase.h
        src/SessionBase.cpp
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ${CONAN_LIBS_BOOST}
            mlogger
)

#target_precompile_headers(mlogger INTERFACE pch/pch.h)