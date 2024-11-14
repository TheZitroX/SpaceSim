//
// Created by zitri on 07.11.2024.
//

#include <box2d/box2d.h>
#include <iostream>
#include "ssSimulationWorld.h"
#include "ssSWDebugDraw.h"

constexpr float GRAVITY = 9.80665;
constexpr float METERS_PER_PIXEL_RATIO = 2.0f;

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

        auto groundShapeDef = b2DefaultShapeDef();
        groundShapeDef.restitution = 0.0f;
        (void) b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
    }

    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type     = b2_dynamicBody;
        bodyDef.position = (b2Vec2) {100.0f, 300.0f};
        m_bodyId = b2CreateBody(m_worldId, &bodyDef);

        b2Polygon  dynamicBox = b2MakeBox(5.0f, 5.0f);
        b2ShapeDef shapeDef   = b2DefaultShapeDef();
        shapeDef.restitution = 0.5f;
        (void) b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);
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
    m_debugDraw = new ssSWDebugDraw(rendererPtr, METERS_PER_PIXEL_RATIO);
}

void ssSimulationWorld::debugDraw()
{
    b2World_Draw(m_worldId, m_debugDraw);
}

void ssSimulationWorld::addRect(float x, float y, float width, float height)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = (b2Vec2) {x / METERS_PER_PIXEL_RATIO, y / METERS_PER_PIXEL_RATIO};
    const auto bodyId = b2CreateBody(m_worldId, &bodyDef);

    b2Polygon  dynamicBox = b2MakeBox(width, height);
    b2ShapeDef shapeDef   = b2DefaultShapeDef();
    shapeDef.restitution = 0.5f;
    (void) b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
}
