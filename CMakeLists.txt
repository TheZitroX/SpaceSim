cmake_minimum_required(VERSION 3.28)

project(SpaceSim)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_ttf)
add_subdirectory(vendor/box2d)

add_subdirectory(base)
add_subdirectory(engine)

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(ENGINE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine")
file(COPY ${RESOURCE_DIR} DESTINATION ${ENGINE_BUILD_DIR})

