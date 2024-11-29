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

    void addHundredRectsScreenToWorld(float x, float y, float width, float height, float dx, float dy);
    void addRectScreenToWorld(float x, float y, float width, float height);
    void createExplosion(float x, float y);
    void renderImGui();
private:
    b2WorldId m_worldId{};
    b2BodyId  m_bodyId{};

    ssSWDebugDraw* m_debugDraw{};
    void addRect(float x, float y, float width, float height);
};
