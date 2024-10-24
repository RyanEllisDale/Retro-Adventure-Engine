// Config:
#pragma once

// Includes:
#include "../Classes/Graphics.h"

class GameEntity
{
protected:
    // Position:
    float m_x{0};
    float m_y{0};

    // To Be Inherited:
    virtual void update(void) {}
    virtual void draw(void) const {}
    
public:
    // Construction
    GameEntity() = default;
    ~GameEntity() = default;
};
