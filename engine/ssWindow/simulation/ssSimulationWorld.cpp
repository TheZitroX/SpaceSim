//
// Created by zitri on 07.11.2024.
//

#include <box2d/box2d.h>
#include <iostream>
#include <imgui.h>

#include "ssSimulationWorld.h"
#include "ssSWDebugDraw.h"

constexpr float GRAVITY                = 9.80665;
//constexpr float GRAVITY                = 0.0;
constexpr float METERS_PER_PIXEL_RATIO = 2.0f;

ssSimulationWorld::ssSimulationWorld()
{
    auto worldDefinition = b2DefaultWorldDef();
    worldDefinition.gravity.y             = GRAVITY;
    worldDefinition.maximumLinearSpeed = 400.0f * b2GetLengthUnitsPerMeter() * 100.0f;
    worldDefinition.restitutionThreshold  = 0.0f;
    m_worldId = b2CreateWorld(&worldDefinition);

    {
        auto groundBodyDefinition = b2DefaultBodyDef();
        groundBodyDefinition.position = b2Vec2(250, 500);
        const auto groundBox      = b2MakeBox(500.0f, 1.0f);
        auto       groundShapeDef = b2DefaultShapeDef();
        groundShapeDef.restitution = 0.0f;

        const auto groundId = b2CreateBody(m_worldId, &groundBodyDefinition);
        (void) b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
    }

    { // spring test
        auto bodyDef = b2DefaultBodyDef();
        bodyDef.type     = b2_dynamicBody;
        bodyDef.position = (b2Vec2) {100.0f, 300.0f};
        auto dynamicBox = b2MakeBox(5.0f, 5.0f);
        auto shapeDef   = b2DefaultShapeDef();
        shapeDef.restitution = 0.5f;
        shapeDef.density     = 0.1f;


        auto bodyDef2 = b2DefaultBodyDef();
        bodyDef2.type     = b2_dynamicBody;
        bodyDef2.position = (b2Vec2) {200.0f, 300.0f};
        auto dynamicBox2 = b2MakeBox(5.0f, 5.0f);
        auto shapeDef2   = b2DefaultShapeDef();
        shapeDef2.restitution = 0.5f;
        shapeDef2.density     = 0.1f;

        auto bodyId = b2CreateBody(m_worldId, &bodyDef);
        auto bodyId2 = b2CreateBody(m_worldId, &bodyDef2);
        (void) b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
        (void) b2CreatePolygonShape(bodyId2, &shapeDef2, &dynamicBox2);

        auto distanceJointDef = b2DefaultDistanceJointDef();
        distanceJointDef.bodyIdA = bodyId;
        distanceJointDef.bodyIdB = bodyId2;
        distanceJointDef.localAnchorA = {0, 0};
        distanceJointDef.localAnchorB = {0, 0};
        distanceJointDef.length = 100;
        distanceJointDef.minLength = 0;
        distanceJointDef.maxLength = 1000;
        distanceJointDef.maxMotorForce = 10;
        distanceJointDef.motorSpeed = 0;
        distanceJointDef.enableMotor = true;
        distanceJointDef.collideConnected = false;
        distanceJointDef.enableSpring = true;
        distanceJointDef.hertz = 1;
        distanceJointDef.dampingRatio = 0.5;
        

        b2CreateDistanceJoint(m_worldId, &distanceJointDef);
    }

    {
        //b2BodyDef bodyDef = b2DefaultBodyDef();
        //bodyDef.type     = b2_dynamicBody;
        //bodyDef.position = (b2Vec2) {100.0f, 300.0f};
        //
        //b2Polygon  dynamicBox = b2MakeBox(5.0f, 5.0f);
        //b2ShapeDef shapeDef   = b2DefaultShapeDef();
        //shapeDef.restitution = 0.5f;
        //shapeDef.density     = 0.1f;
        //
        //m_bodyId = b2CreateBody(m_worldId, &bodyDef);
        //(void) b2CreatePolygonShape(m_bodyId, &shapeDef, &dynamicBox);
    }

    {
        //constexpr float positionX = 300.0f;
        //constexpr float positionY = 400.0f;
        //constexpr float width     = 600.0f;
        //constexpr float height    = 60.0f;
        //constexpr float cubeSize  = 1.0f;
        //
        //// fill the space with rectangles
        //constexpr int columns = width / cubeSize;
        //constexpr int rows    = height / cubeSize;
        //
        //for (int i = 0; i < columns; ++i)
        //{
        //    for (int j = 0; j < rows; ++j)
        //    {
        //        addRect(positionX + i * cubeSize, positionY + j * cubeSize, cubeSize / 2, cubeSize / 2);
        //    }
        //}
    }
    {
        // draw a circle
        //b2ShapeDef     shapeDef = b2DefaultShapeDef();
        //
        //const auto circlePolygon = b2MakeRoundedBox(50, 50, 100);
        //
        //b2BodyDef bodyDef = b2DefaultBodyDef();
        //bodyDef.type      = b2_dynamicBody;
        //bodyDef.position  = (b2Vec2) {400.0f, 300.0f};
        //bodyDef.isEnabled = true;
        //
        //const auto bodyId  = b2CreateBody(m_worldId, &bodyDef);
        //(void) b2CreatePolygonShape(bodyId, &shapeDef, &circlePolygon);
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

void ssSimulationWorld::addHundredRectsScreenToWorld(float x, float y, float width, float height, float dx = 0,
                                                     float dy = 0)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type              = b2_dynamicBody;
    bodyDef.position          = (b2Vec2) {x / METERS_PER_PIXEL_RATIO, y / METERS_PER_PIXEL_RATIO};
    bodyDef.isBullet          = false;
    bodyDef.allowFastRotation = true;
    bodyDef.linearVelocity    = {dx, dy};
    b2Polygon  dynamicBox = b2MakeBox(width, height);
    b2ShapeDef shapeDef   = b2DefaultShapeDef();
    shapeDef.friction    = 1.0f;
    shapeDef.restitution = 0.0f;
    shapeDef.density     = 1000.0f;


    for (int i = 0; i < 1; ++i)
    {
        const auto bodyId = b2CreateBody(m_worldId, &bodyDef);
        (void) b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
    }
}

void ssSimulationWorld::addRect(float x, float y, float width, float height)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = (b2Vec2) {x, y};
    const auto bodyId = b2CreateBody(m_worldId, &bodyDef);

    b2Polygon  dynamicBox = b2MakeBox(width, height);
    b2ShapeDef shapeDef   = b2DefaultShapeDef();
    shapeDef.restitution = 1.0f;
    shapeDef.density     = 1.225f;
    (void) b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
}

void ssSimulationWorld::createExplosion(float x, float y)
{
    const auto explosionCenter = b2Vec2(x / METERS_PER_PIXEL_RATIO, y / METERS_PER_PIXEL_RATIO);

    auto explosionDef = b2DefaultExplosionDef();
    explosionDef.position         = explosionCenter;
    explosionDef.radius           = 100.0f;
    explosionDef.impulsePerLength = -1.0f;
    explosionDef.falloff          = 300.0f;

    b2World_Explode(m_worldId, &explosionDef);
}

void ssSimulationWorld::renderImGui()
{
    ImGui::Begin("SpaceSim Menu");

    addCollapsingHeaderSimulation();

    addCollapsingHeaderInfo();

    ImGui::End();
}

void addTableRow(const char* name, const char* value)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%s", name);
    ImGui::TableNextColumn();
    ImGui::Text("%s", value);
}

void ssSimulationWorld::addCollapsingHeaderSimulation()
{
    if (!ImGui::CollapsingHeader("Simulation"))
        return;

    static float timeStep = 1.0f / 60.0f;
    ImGui::SliderFloat("Time step", &timeStep, 0.0f, 10.0f);

    static int subStepCount = 4;
    ImGui::SliderInt("Substep count", &subStepCount, 1, 50);

    if (ImGui::Button("Step"))
    {
        step(timeStep, subStepCount);
    }
}

void ssSimulationWorld::addCollapsingHeaderInfo()
{
    if (!ImGui::CollapsingHeader("Info"))
        return;

    // info about the world
    const auto worldCounters = b2World_GetCounters(m_worldId);

    // table
    ImGui::BeginTable("WorldCounters", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Value");
    ImGui::TableHeadersRow();
    addTableRow("Body count", std::to_string(worldCounters.bodyCount).c_str());
    addTableRow("Shape count", std::to_string(worldCounters.shapeCount).c_str());
    addTableRow("Contact count", std::to_string(worldCounters.contactCount).c_str());
    addTableRow("Joint count", std::to_string(worldCounters.jointCount).c_str());
    addTableRow("Island count", std::to_string(worldCounters.islandCount).c_str());
    addTableRow("Static tree height", std::to_string(worldCounters.staticTreeHeight).c_str());
    addTableRow("Byte count", std::to_string(worldCounters.byteCount).c_str());
    addTableRow("Task count", std::to_string(worldCounters.taskCount).c_str());
    ImGui::EndTable();
}

