//
// Created by zitri on 07.11.2024.
//

#include <box2d/box2d.h>
#include <iostream>
#include "ssSimulationWorld.h"
#include "ssSWDebugDraw.h"

constexpr float GRAVITY                = 9.80665;
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
        //b2BodyDef bodyDef = b2DefaultBodyDef();
        //bodyDef.type     = b2_dynamicBody;
        //bodyDef.position = (b2Vec2) {100.0f, 300.0f};
        //m_bodyId = b2CreateBody(m_worldId, &bodyDef);
        //
        //b2Polygon  dynamicBox = b2MakeBox(5.0f, 5.0f);
        //b2ShapeDef shapeDef   = b2DefaultShapeDef();
        //shapeDef.restitution = 0.5f;
        //shapeDef.density     = 0.1f;
        //(void) b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);
    }

    {
        constexpr float positionX = 300.0f;
        constexpr float positionY = 300.0f;
        constexpr float width     = 10.0f;
        constexpr float height    = 10.0f;
        constexpr float cubeSize  = 0.2f;

        // fill the space with rectangles
        constexpr int columns = width / cubeSize;
        constexpr int rows    = height / cubeSize;

        for (int i = 0; i < columns; ++i)
        {
            for (int j = 0; j < rows; ++j)
            {
                addRect(positionX + i * 2*cubeSize, positionY + j * 2*cubeSize, cubeSize, cubeSize);
            }
        }
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

void ssSimulationWorld::addRectScreenToWorld(float x, float y, float width, float height)
{
    addRect(x / METERS_PER_PIXEL_RATIO, y / METERS_PER_PIXEL_RATIO, width, height);
}

void ssSimulationWorld::addRect(float x, float y, float width, float height)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = (b2Vec2) {x, y};
    const auto bodyId = b2CreateBody(m_worldId, &bodyDef);

    b2Polygon  dynamicBox = b2MakeBox(width, height);
    b2ShapeDef shapeDef   = b2DefaultShapeDef();
    shapeDef.restitution = 0.5f;
    shapeDef.density     = 1.225f;
    (void) b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
}

void ssSimulationWorld::createExplosion(float x, float y)
{
    const auto explosionCenter = b2Vec2(x / METERS_PER_PIXEL_RATIO, y / METERS_PER_PIXEL_RATIO);

    auto explosionDef = b2DefaultExplosionDef();
    explosionDef.position         = explosionCenter;
    explosionDef.radius           = 100.0f;
    explosionDef.impulsePerLength = 10.0f;
    explosionDef.falloff          = 300.0f;

    b2World_Explode(m_worldId, &explosionDef);
}
