cmake_minimum_required(VERSION 3.21)

set(APP_NAME unit-testing)

add_executable(${APP_NAME})

target_sources(${APP_NAME} PUBLIC
        main.cpp
        controller/controller.cpp
)

target_link_libraries(${APP_NAME}
        ${CONAN_LIBS}
        cancel_controller
)

add_test(NAME ${APP_NAME} COMMAND $<TARGET_FILE:${APP_NAME}>)