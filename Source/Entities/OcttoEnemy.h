// enemy number 1 with basic finite state machine AI
// simple monster creature meant to be pretty easy

#pragma once

// Include:
#include "Base Entities/Character.h" // Parent

// defines AI behaviour
enum OcttoState
{
    m_moving,
    m_shooting
};

class OcttoEnemy final : public Character
{
private:
    // Animations:
    Animation m_walkDown;
    Animation m_walkSide;

    OcttoState m_currentState{ m_moving };
    sf::Clock m_movementClock;
    sf::Clock m_shootClock;
    float m_movementTime{ 3.0f };
    float m_shootTime{ 6.0f };
    float m_projectileForce{ 120.0f };

    // private functions:
    void pickDirection(void);
    bool collisionWithMap(void) override;
public:
    // Construction:
    OcttoEnemy();

    // Member Function:
    void update(const double& a_deltaTime, const float& a_simulationTimeScalar) override;
};