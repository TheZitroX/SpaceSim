//
// Created by zitri on 07.11.2024.
//

#include <iostream>
#include "ssSimulationWorld.h"

constexpr float GRAVITY = 9.80665;

ssSimulationWorld::ssSimulationWorld()
{
    auto worldDefinition = b2DefaultWorldDef();
    worldDefinition.gravity.y = GRAVITY;
    m_worldId = b2CreateWorld(&worldDefinition);

    auto groundBodyDefinition = b2DefaultBodyDef();
    groundBodyDefinition.position = b2Vec2(0, 100);

    const auto groundId  = b2CreateBody(m_worldId, &groundBodyDefinition);
    const auto groundBox = b2MakeBox(50.0f, 10.0f);

    const auto groundShapeDef = b2DefaultShapeDef();
    (void) b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = (b2Vec2) {0.0f, 0.0f};
    m_bodyId = b2CreateBody(m_worldId, &bodyDef);

    b2Polygon  dynamicBox = b2MakeBox(10.0f, 10.0f);
    b2ShapeDef shapeDef   = b2DefaultShapeDef();
    shapeDef.density  = 0.01f;
    shapeDef.friction = 0.3f;
    b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);
}

ssSimulationWorld::~ssSimulationWorld()
{
    b2DestroyWorld(m_worldId);
}

void ssSimulationWorld::step(const float timeStep, const int subStepCount)
{
    b2World_Step(m_worldId, timeStep, subStepCount);
}

void ssSimulationWorld::setRenderer(SDL_Renderer* rendererPtr)
{
    m_rendererPtr = rendererPtr;
    m_debugDraw   = ssSWDebugDraw(m_rendererPtr, 1.0f);
}

void ssSimulationWorld::debugDraw()
{
    b2World_Draw(m_worldId, &m_debugDraw);
}
