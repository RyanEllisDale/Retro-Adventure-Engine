// derived entity class with Physics and collisions,
// used to create entities that can move and are not purely visual.
// parent of the character entity class

// completely protected class meant to be derived from,
// all functions are virtual and overwritable to allow for custom,
// collision detection and functionality inside the phsyics entity

#pragma once

// Includes:
#include "Entity.h"
#include "../../Simulation/Physics/Physics.hpp"

class PhysicsEntity : public Entity
{
protected:
    // Physics:
    MyVector m_velocity{ 0,0 };
    float m_movementForce{ 55000.0f };
    float m_massG{100.0f};
    float m_dragCoefficient{0.99f};
    float m_acceleration{0.0f};

    // protected functions:
    virtual bool collisionScript(void);
    virtual const Entity* collisionWithWorldEntities(void);
    virtual bool collisionWithMap(void);
    virtual void move(const MyVector& a_force, const double& a_deltaTime);
};