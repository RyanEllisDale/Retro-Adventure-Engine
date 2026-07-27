// stationary sentry enemy, shoots based off a minimilist timer,
// has a bit more health than the octto enemy but appears less often in less levels

#pragma once

// Include:
#include "Base Entities/Character.h" // Parent

class CopterEnemy final : public Character
{
private:
    // Animations:
    Animation m_idle;
    sf::Clock m_shootClock;
    float m_shootTime{ 3.0f };
    float m_projectileForce{ 120.0f };


public:
    // Construction:
    CopterEnemy();

    // Member Function:
    void update(const double& a_deltaTime, const float& a_simulationTimeScalar) override;
};