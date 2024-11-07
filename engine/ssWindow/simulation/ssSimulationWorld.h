//
// Created by zitri on 07.11.2024.
//

#pragma once

#include <box2d/box2d.h>

class ssSimulationWorld {
public:
    ssSimulationWorld();

    ~ssSimulationWorld();

    void step(float timeStep, int subStepCount);

    inline b2Vec2 getPosition() const { return b2Body_GetPosition(m_bodyId); }

private:
    b2WorldId m_worldId{};
    b2BodyId m_bodyId;
};
