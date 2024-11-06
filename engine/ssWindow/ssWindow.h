//
// Created by zitri on 05.11.2024.
//

#pragma once

#include <SDL3/SDL.h>
#include <thread>

class ssWindow {
public:
    ssWindow();

    ~ssWindow();

    void run();

private:
    bool m_isFullscreen = false;
    bool m_running = false;

    SDL_Window *m_windowPtr{};
    SDL_Renderer *m_rendererPtr{};

    void draw();
    void runRenderLoop();
    std::thread m_renderThread;

    void update();

    void handleSDLEvents();
    void handleSDLKeyDown(const SDL_KeyboardEvent &key);


};
