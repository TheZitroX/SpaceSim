//
// Created by zitri on 07.11.2024.
//

#include <box2d/box2d.h>
#include <iostream>
#include "ssSimulationWorld.h"
#include "ssSWDebugDraw.h"

constexpr float GRAVITY = 9.80665;

ssSimulationWorld::ssSimulationWorld()
{
    auto worldDefinition = b2DefaultWorldDef();
    worldDefinition.gravity.y = GRAVITY;
    m_worldId = b2CreateWorld(&worldDefinition);

    {
        auto groundBodyDefinition = b2DefaultBodyDef();
        groundBodyDefinition.position = b2Vec2(250, 500);

        const auto groundId  = b2CreateBody(m_worldId, &groundBodyDefinition);
        const auto groundBox = b2MakeBox(500.0f, 1.0f);

        const auto groundShapeDef = b2DefaultShapeDef();
        auto       shapeId        = b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
    }

    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type     = b2_dynamicBody;
        bodyDef.position = (b2Vec2) {100.0f, 0.0f};
        m_bodyId = b2CreateBody(m_worldId, &bodyDef);

        b2Polygon  dynamicBox = b2MakeBox(5.0f, 5.0f);
        b2ShapeDef shapeDef   = b2DefaultShapeDef();
        shapeDef.density  = 0.01f;
        shapeDef.friction = 0.3f;
        auto shapeID = b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);
    }
}

ssSimulationWorld::~ssSimulationWorld()
{
    b2DestroyWorld(m_worldId);

    delete m_debugDraw;
}

void ssSimulationWorld::step(const float deltaTime, const int subStepCount)
{
    b2World_Step(m_worldId, deltaTime, subStepCount);
}

void ssSimulationWorld::setRenderer(SDL_Renderer* rendererPtr)
{
    m_debugDraw = new ssSWDebugDraw(rendererPtr, 1.0f);
}

void ssSimulationWorld::debugDraw()
{
    b2World_Draw(m_worldId, m_debugDraw);
}
