//
// Created by zitri on 05.11.2024.
//

#pragma once

#include <SDL3/SDL.h>
#include <thread>
#include <vector>
#include "simulation/ssSimulationWorld.h"

class ssWindow {
public:
    ssWindow();

    ~ssWindow();

    void run();

private:
    bool m_isFullscreen = false;
    std::atomic<bool> m_running = false;

    SDL_Window *m_windowPtr{};
    SDL_Renderer *m_rendererPtr{};

    struct Points {
        float x;
        float y;
    };
    struct MouseMotion {
        SDL_FPoint velocity{};
        std::vector<SDL_FPoint> vecPoints{};
    };
    MouseMotion m_mouseMotion;
    std::mutex m_mouseMotionMutex;
    std::atomic<bool> m_mouseMotionActive = false;

    void draw();
    void runRenderLoop();
    std::thread m_renderThread;

    void update();

    void handleSDLEvents();
    void handleSDLKeyDown(const SDL_KeyboardEvent &key);


    void handleSDLMouseMotion(const SDL_MouseMotionEvent &motion);

    void drawMouseMotion();

    void drawFPS();

    ssSimulationWorld m_simulationWorld{};

    void drawWorld();
};
