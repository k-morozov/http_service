cmake_minimum_required(VERSION 3.21)

project(cancel_controller CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC src)

target_sources(${PROJECT_NAME}
    PUBLIC
        src/Controller.cpp
)

include_directories(SYSTEM ${CONAN_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${CONAN_LIBS_BOOST}
        mlogger
)