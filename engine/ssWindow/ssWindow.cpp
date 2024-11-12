//
// Created by zitri on 05.11.2024.
//

#include "ssWindow.h"

#include <memory>
#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>

ssWindow::ssWindow()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (!TTF_Init())
    {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_isFullscreen = false;
    m_windowPtr    = SDL_CreateWindow(
        "Hello, SDL3!",
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );
    if (!m_windowPtr)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_rendererPtr = SDL_CreateRenderer(m_windowPtr, nullptr);
    if (!m_rendererPtr)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    m_simulationWorld.setRenderer(m_rendererPtr);
}

ssWindow::~ssWindow()
{
    if (m_renderThread.joinable())
        m_renderThread.join();

    SDL_DestroyRenderer(m_rendererPtr);
    SDL_DestroyWindow(m_windowPtr);
    TTF_Quit();
    SDL_Quit();
}

void ssWindow::run()
{
    m_running = true;

//#define USE_MULTITHREADING
#ifdef USE_MULTITHREADING
    m_renderThread = std::thread(&ssWindow::runRenderLoop, this);
#endif

    while (m_running)
    {
        handleSDLEvents();
        update();
#ifndef USE_MULTITHREADING
        draw();
#endif
    }
}


void ssWindow::draw()
{
    SDL_SetRenderDrawColor(m_rendererPtr, 0, 0, 0, 255);
    SDL_RenderClear(m_rendererPtr);

//    SDL_FRect rect = {100, 100, 100, 100};
//    SDL_SetRenderDrawColor(m_rendererPtr, 255, 0, 255, 255);
//    SDL_RenderFillRect(m_rendererPtr, &rect);

    drawMouseMotion();

    drawFPS();

    m_simulationWorld.debugDraw();

    SDL_RenderPresent(m_rendererPtr);
}

void ssWindow::update()
{
    m_simulationWorld.step(1.0f / 60.0f, 1);

    // todo implement float deltaTime
    SDL_Delay(16);
}

void ssWindow::handleSDLEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        const auto eventType = event.type;
        switch (eventType)
        {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                handleSDLKeyDown(event.key);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                m_mouseMotionActive = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                m_mouseMotionActive = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                handleSDLMouseMotion(event.motion);
                break;
            default:
                break;
        }
    }
}

void ssWindow::handleSDLKeyDown(const SDL_KeyboardEvent& key)
{
    const auto keyType = key.scancode;
    switch (keyType)
    {
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

void ssWindow::runRenderLoop()
{
    while (m_running)
    {
        draw();
    }
}

void ssWindow::handleSDLMouseMotion(const SDL_MouseMotionEvent& motion)
{
    std::lock_guard<std::mutex> lock(m_mouseMotionMutex);

    if (!m_mouseMotionActive)
        return;

    m_mouseMotion.velocity = {motion.xrel, motion.yrel};
    m_mouseMotion.vecPoints.push_back({motion.x, motion.y});
}

void ssWindow::drawMouseMotion()
{
    std::lock_guard<std::mutex> lock(m_mouseMotionMutex);

    if (m_mouseMotion.vecPoints.empty())
        return;

    SDL_SetRenderDrawColor(m_rendererPtr, 255, 255, 255, 255);
    SDL_RenderPoints(m_rendererPtr, m_mouseMotion.vecPoints.data(), m_mouseMotion.vecPoints.size());

    // Draw velocity vector
    SDL_SetRenderDrawColor(m_rendererPtr, 255, 0, 0, 255);
    const auto success = SDL_RenderLine(
        m_rendererPtr,
        m_mouseMotion.vecPoints.back().x,
        m_mouseMotion.vecPoints.back().y,
        m_mouseMotion.vecPoints.back().x + m_mouseMotion.velocity.x,
        m_mouseMotion.vecPoints.back().y + m_mouseMotion.velocity.y
    );
    if (!success)
    {
        std::cerr << "SDL_RenderLine failed: " << SDL_GetError() << std::endl;
        SDL_ClearError();
    }
}

void ssWindow::drawFPS()
{
    const auto text = const_cast<char*>("Hello World!");
    const auto font = TTF_OpenFont("res/font/arial.ttf", 2000);
    if (!font)
    {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    const SDL_Color color{255, 0, 255, 255};
    const auto      textSurface = TTF_RenderText_Solid(font, text, strlen(text), color);
    if (!textSurface)
    {
        std::cerr << "TTF_RenderText_Solid failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    const auto textTexture = SDL_CreateTextureFromSurface(m_rendererPtr, textSurface);
    if (!textTexture)
    {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        exit(1);
    }

    // Draw text
    SDL_RenderTexture(m_rendererPtr, textTexture, nullptr, nullptr);

    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}
