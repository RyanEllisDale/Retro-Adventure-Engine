// Config:
#pragma once

// Includes:
#include "Character.h"

class Player : protected Character
{
private:
    // Animation:
    AnimationData m_walkUp;
    AnimationData m_walkDown;
    AnimationData m_walkSide;
    
public:
    // Construction:
    Player(const unsigned short& a_x = 0, const unsigned short& a_y = 0);
    ~Player() override = default;
    
    void update(void) override;
    void draw(void) const override;
};
