cmake_minimum_required(VERSION 3.21)
project(analytics)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CONAN_DISABLE_CHECK_COMPILER True)
conan_basic_setup()

list(APPEND CONAN_LIBS_BOOST boost_log boost_log_setup)

include_directories(SYSTEM
        ${CONAN_INCLUDE_DIRS_BOOST}
)

include_directories(sdk)

add_subdirectory(sdk)
add_subdirectory(app)
