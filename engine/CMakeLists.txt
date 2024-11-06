cmake_minimum_required(VERSION 3.28)
project(Engine)

set(CMAKE_CXX_STANDARD 23)


add_executable(${PROJECT_NAME} main.cpp
        ssWindow/ssWindow.h
        ssWindow/ssWindow.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Base SDL3::SDL3)
target_include_directories(${PROJECT_NAME} PUBLIC Base SDL3::SDL3)
target_link_libraries(${PROJECT_NAME} PUBLIC Base SDL3_ttf::SDL3_ttf)
target_include_directories(${PROJECT_NAME} PUBLIC Base SDL3_ttf::SDL3_ttf)
