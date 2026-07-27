// the basis for the player and enemies, 
// a character entitiy type that has health and death functionality
// Character encompasses game objects with hit points and state machine
// die is virtual so all character can determine how they die (partical effects, etc)

#pragma once

// Includes:
#include "PhysicsEntity.h"

class Character : public PhysicsEntity
{
protected:
    // Health:
    int m_maxHealth{1};
    int m_currentHealth{m_maxHealth};

    // Protected Functions:
    virtual void Die(void);

public:
    void takeDamage(const int& a_damage);
};
