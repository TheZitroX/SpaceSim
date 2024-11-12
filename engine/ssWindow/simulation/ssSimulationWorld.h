//
// Created by zitri on 07.11.2024.
//

#pragma once

#include <box2d/box2d.h>
#include "ssSWDebugDraw.h"

class ssSimulationWorld
{
public:
    explicit ssSimulationWorld();

    ~ssSimulationWorld();

    void step(float timeStep, int subStepCount);

    void setRenderer(SDL_Renderer* rendererPtr);

    void debugDraw();

private:
    b2WorldId m_worldId{};
    b2BodyId  m_bodyId{};

    SDL_Renderer* m_rendererPtr{};
    ssSWDebugDraw m_debugDraw{};
};
