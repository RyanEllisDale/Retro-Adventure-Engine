// Config
#pragma once

// Include
#include "GameEntity.h"

enum Direction
{
    Left,
    Right,
    Up,
    Down,
    Null
};

class Character : protected GameEntity
{
protected:
    // Member Variables:
    short m_health{0};
    Direction m_direction{Left};
    float m_movementSpeed{0};
    Animation m_animation;
    
    // Functions:
    void move(const Direction& a_direction);
    void move(const short& a_horizontalDirection = 0, const short& a_verticalDirection = 0);
    void draw(void) const override;

public:
    // Construction:
    Character(const unsigned short& a_x = 0, const unsigned short& a_y = 0);
    virtual ~Character(void) = default;
};
