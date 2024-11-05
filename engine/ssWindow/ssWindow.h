//
// Created by zitri on 05.11.2024.
//

#pragma once

#include <SDL3/SDL.h>

class ssWindow {
public:
    ssWindow();
    ~ssWindow();

private:
    bool m_isFullscreen = false;

    SDL_Window* m_windowPtr{};
    SDL_Renderer* m_rendererPtr{};
};
