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
            1280,
            720,
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
    if (m_renderThread.joinable())
        m_renderThread.join();

    SDL_DestroyRenderer(m_rendererPtr);
    SDL_DestroyWindow(m_windowPtr);
    SDL_Quit();
}

void ssWindow::run() {
    m_running = true;

    m_renderThread = std::thread(&ssWindow::runRenderLoop, this);

    while (m_running) {
        handleSDLEvents();
        update();
    }
}


void ssWindow::draw() {
    SDL_SetRenderDrawColor(m_rendererPtr, 0, 0, 0, 255);
    SDL_RenderClear(m_rendererPtr);

    SDL_FRect rect = { 100, 100, 100, 100 };
    SDL_SetRenderDrawColor(m_rendererPtr, 255, 0, 255, 255);
    SDL_RenderFillRect(m_rendererPtr, &rect);

    

    SDL_RenderPresent(m_rendererPtr);
}

void ssWindow::update() {
}

void ssWindow::handleSDLEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        const auto eventType = event.type;
        switch (eventType) {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                handleSDLKeyDown(event.key);
                break;
            default:
                break;
        }
    }
}

void ssWindow::handleSDLKeyDown(const SDL_KeyboardEvent &key) {
    const auto keyType = key.scancode;
    switch (keyType) {
        case SDL_SCANCODE_ESCAPE:
            m_running = false;
            break;
        case SDL_SCANCODE_F11:
            m_isFullscreen = !m_isFullscreen;
            SDL_SetWindowFullscreen(m_windowPtr, m_isFullscreen);
        default:
            break;
    }
}

void ssWindow::runRenderLoop() {
    while (m_running) {
        draw();
    }
}
