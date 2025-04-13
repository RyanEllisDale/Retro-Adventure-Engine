// Main player character:
// has lots of custom logic and is friends with the world system class,
// the only persistent object, meaning it is not re-created upon entering,
// and / or exiting a room / level 
// is specifically built in the world build function

#pragma once

// Include:
#include "Base Entities/Character.h" // Parent:
#include "../../Simulation/InputSystem.h"

// Forward Declare:
class Sword;

class Player final : public Character, public InputInterface
{
private:
    friend class Sword;

    float m_swordShootingForce{110.0f};
    sf::Clock m_swordClock;
    sf::Time m_swordCooldown{sf::seconds(2.5f)};
    bool m_hasSword{false};
    
    // Animations:
    Animation m_walkDown;
    Animation m_walkUp;
    Animation m_walkSide;

    void Die(void) override;
public:
    // Construction:
    Player();

    // Member Function:
    void update(const double& a_deltaTime, const float& a_simulationTimeScalar) override;
    void updateInput(const std::vector<Input>& a_inputs) override;
};
