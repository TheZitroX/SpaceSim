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
        ${ABSOLUT_IMGUI_PATH}/imgui_demo.cpp
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

if(WIN32)
    set(LIB_EXT ".dll")
elseif(APPLE)
    set(LIB_EXT ".dylib")
else()
    set(LIB_EXT ".so")
endif()

set(LIBRARY_DIRS
    "${CMAKE_BINARY_DIR}/vendor/SDL"
    "${CMAKE_BINARY_DIR}/vendor/SDL_ttf"
    "${CMAKE_BINARY_DIR}/vendor/box2d/src"
)

set(TARGET_NAME ${PROJECT_NAME})

set(FILES_TO_COPY "")

foreach(DIR ${LIBRARY_DIRS})
    file(GLOB SHARED_LIBS "${DIR}/*${LIB_EXT}")

    foreach(LIB_FILE ${SHARED_LIBS})
        list(APPEND FILES_TO_COPY "${LIB_FILE}")
    endforeach()
endforeach()

if(FILES_TO_COPY)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${FILES_TO_COPY}
            "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )
endif()
