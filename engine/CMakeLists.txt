cmake_minimum_required(VERSION 3.28)
project(Engine)

set(CMAKE_CXX_STANDARD 23)

set(ABSOLUT_IMGUI_PATH "${CMAKE_SOURCE_DIR}/${IMGUI_PATH}")

add_executable(${PROJECT_NAME} main.cpp
        ssWindow/ssWindow.h
        ssWindow/ssWindow.cpp
        ssWindow/simulation/ssSimulationWorld.h
        ssWindow/simulation/ssSimulationWorld.cpp
        ssWindow/simulation/ssSWDebugDraw.h
        ssWindow/simulation/ssSWDebugDraw.cpp

        # imgui
        ${ABSOLUT_IMGUI_PATH}/imgui.cpp
        ${ABSOLUT_IMGUI_PATH}/imgui_draw.cpp
        ${ABSOLUT_IMGUI_PATH}/imgui_widgets.cpp
        ${ABSOLUT_IMGUI_PATH}/imgui_tables.cpp
        ${ABSOLUT_IMGUI_PATH}/backends/imgui_impl_sdl3.cpp
        ${ABSOLUT_IMGUI_PATH}/backends/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC Base
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        box2d::box2d
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${ABSOLUT_IMGUI_PATH}
        ${ABSOLUT_IMGUI_PATH}/backends
        Base
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        box2d::box2d
)
