cmake_minimum_required(VERSION 3.28)

project(SpaceSim)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_ttf)
add_subdirectory(vendor/box2d)

set(IMGUI_PATH  "vendor/imgui")
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
add_library("ImGui" STATIC ${IMGUI_SOURCES})

add_subdirectory(base)
add_subdirectory(engine)

set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(ENGINE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/engine")
file(COPY ${RESOURCE_DIR} DESTINATION ${ENGINE_BUILD_DIR})

# add every file or folder from assets to ENGINE_BUILD_DIR/assets
file(GLOB_RECURSE ASSETS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
foreach(ASSET_FILE ${ASSETS_FILES})
    file(COPY ${ASSET_FILE} DESTINATION ${ENGINE_BUILD_DIR}/assets)
endforeach()




