//
// Created by zitri on 07.11.2024.
//

#pragma once

#include <box2d/box2d.h>

class SDL_Renderer;
class ssSWDebugDraw;

class ssSimulationWorld
{
public:
    explicit ssSimulationWorld();

    ~ssSimulationWorld();

    void step(float deltaTime, int subStepCount);

    void setRenderer(SDL_Renderer* rendererPtr);

    void debugDraw();

private:
    b2WorldId m_worldId{};
    b2BodyId  m_bodyId{};

    ssSWDebugDraw* m_debugDraw{};
};
