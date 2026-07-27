// simple projectile class that does damage to characters,
// has an initial velocity that slowly decreases due to air and friction,
// can destroy/kill character classes, main gameplay feature

// Config:
#pragma once

// Include:
#include "../RedirectCout.h"
#include "Base Entities/PhysicsEntity.h"

class Projectile : public PhysicsEntity
{
private:
    int m_damage{ 1 };
    Animation m_vertical;
    Animation m_horizontal;
    void move(const MyVector& a_force, const double& a_deltaTime) override;
    const Entity* collisionWithWorldEntities(void) override;

public:
    // Construction:
    Projectile() = default;
    Projectile(const MyVector& a_position, Entity*& a_owner, const MyVector& a_initialForce);
    
    // Key Functions:
    void update(const double& a_deltaTime, const float& a_simulationTimeScalar) override;
};
