//
// Created by zitri on 05.11.2024.
//

#include "ssWindow.h"

#include <memory>
#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include <mutex>

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
        | SDL_WINDOW_FULLSCREEN
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
    SDL_SetRenderDrawBlendMode(m_rendererPtr, SDL_BLENDMODE_BLEND);

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
    float deltaTime = 0;

//#define USE_MULTITHREADING
#ifdef USE_MULTITHREADING
    m_renderThread = std::thread(&ssWindow::runRenderLoop, this);
#endif

    while (m_running)
    {
        const auto startTimeNS = SDL_GetTicksNS();

        handleSDLEvents();
        update(deltaTime);
#ifndef USE_MULTITHREADING
        draw();
#endif

        const auto endTimeNS = SDL_GetTicksNS();
        deltaTime = static_cast<float>(endTimeNS - startTimeNS) / SDL_NS_PER_SECOND;
    }
}


void ssWindow::draw()
{
    SDL_SetRenderDrawColor(m_rendererPtr, 0, 0, 0, 255);
    SDL_RenderClear(m_rendererPtr);

//    SDL_FRect rect = {100, 100, 100, 100};
//    SDL_SetRenderDrawColor(m_rendererPtr, 255, 0, 255, 255);
//    SDL_RenderFillRect(m_rendererPtr, &rect);

    m_simulationWorld.debugDraw();
    //drawMouseMotion();
    //drawFPS();

    SDL_RenderPresent(m_rendererPtr);
}

void ssWindow::update(const float deltaTime)
{
    const auto timeStep = deltaTime > 1.0f/60.0f ? 1.0f/60.0f : deltaTime;
    m_simulationWorld.step(timeStep, 4);
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
            {
                const auto mouseEvent = event.button;
                if (mouseEvent.button == SDL_BUTTON_LEFT)
                {
                    m_mouseMotionActive = true;
                    handleSDLMouseMotion(event.motion);
                }
                else if (mouseEvent.button == SDL_BUTTON_RIGHT)
                {
                    m_simulationWorld.createExplosion(mouseEvent.x, mouseEvent.y);
                }

                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
                m_mouseMotionActive = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
            {
                const auto mouseMotionEvent = event.motion;
                handleSDLMouseMotion(mouseMotionEvent);
                break;
            }
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

    //m_mouseMotion.velocity = {motion.xrel, motion.yrel};
    //m_mouseMotion.vecPoints.push_back({motion.x, motion.y});

    m_simulationWorld.addHundredRectsScreenToWorld(motion.x, motion.y, 10, 10, motion.xrel*20, motion.yrel*20);
    //m_simulationWorld.addRect(motion.x, motion.y, 0.1, 0.1);
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
