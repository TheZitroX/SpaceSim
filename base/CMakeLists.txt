cmake_minimum_required(VERSION 3.28)

project(Base)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES defines.h base.c)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/
)
