//
// Created by zitri on 05.11.2024.
//

#include "ssWindow.h"

#include <memory>
#include <SDL3/SDL.h>
#include <iostream>

ssWindow::ssWindow() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_isFullscreen = false;
    m_windowPtr = SDL_CreateWindow(
            "Hello, SDL3!",
            800,
            600,
            SDL_WINDOW_RESIZABLE
    );

    if (!m_windowPtr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_rendererPtr = SDL_CreateRenderer(m_windowPtr, "Hello");

    auto running = true;
    while (running) {
        auto start = SDL_GetTicks();
        SDL_RenderClear(m_rendererPtr);
        SDL_RenderPresent(m_rendererPtr);
        auto end = SDL_GetTicks();
        auto elapsed = end - start;
        if (elapsed < 16) {
            SDL_Delay(16 - elapsed);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.key.scancode == SDL_SCANCODE_F11) {
                if (event.key.down) {
                    m_isFullscreen = !m_isFullscreen;
                    SDL_SetWindowFullscreen(m_windowPtr, m_isFullscreen);
                }
            }
        }
    }
}

ssWindow::~ssWindow() {
    SDL_DestroyRenderer(m_rendererPtr);
    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
}
