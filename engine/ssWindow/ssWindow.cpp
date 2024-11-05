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

    m_rendererPtr = SDL_CreateRenderer(m_windowPtr, nullptr);
    if (!m_rendererPtr) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

ssWindow::~ssWindow() {
    SDL_DestroyRenderer(m_rendererPtr);
    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
}

void ssWindow::run() {
    m_running = true;
    while (m_running) {
        update();
        draw();
    }
}

int color_r = 0;
int color_b = 0;
int color_g = 0;

void ssWindow::draw() {
    SDL_SetRenderDrawColor(m_rendererPtr, color_r, color_b, color_g, 255);
    SDL_RenderClear(m_rendererPtr);
    SDL_RenderPresent(m_rendererPtr);

    color_r++;
    color_b++;
    color_g++;
    if (color_r > 255) color_r = 0;
    if (color_b > 255) color_b = 0;
    if (color_g > 255) color_g = 0;
}

void ssWindow::update() {
    std::cout << "resolution: " << SDL_GetWindowSurface(m_windowPtr)->w << "x" << SDL_GetWindowSurface(m_windowPtr)->h
              << std::endl;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        } else if (event.key.scancode == SDL_SCANCODE_F11) {
            if (event.key.down) {
                m_isFullscreen = !m_isFullscreen;
                SDL_SetWindowFullscreen(m_windowPtr, m_isFullscreen);
            }
        }
    }
}
