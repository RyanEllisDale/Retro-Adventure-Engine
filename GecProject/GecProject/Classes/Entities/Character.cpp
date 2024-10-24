// Include
#include "Character.h"

#include <iostream>

// Construction:
Character::Character(const unsigned short& a_x, const unsigned short& a_y)
{
    m_x = a_x;
    m_y = a_y;
}

// Functions:
void Character::move(const Direction& a_direction)
{
    switch (a_direction)
    {
    case Left:
        m_direction = Left;
        m_x = m_x - m_movementSpeed;
        break;

    case Right:
        m_direction = Right;
        m_x = m_x + m_movementSpeed;
        break;

    case Up:
        m_direction = Up;
        m_y = m_y - m_movementSpeed;
        break;

    case Down:
        m_direction = Down;
        m_y = m_y + m_movementSpeed;
        break;
    
    default:
        std::cerr << "Invalid Direction Case : Character Move \n";
        break;
    }
}

void Character::move(const short& a_horizontalDirection, const short& a_verticalDirection)
{
    // Error Handling:
    if (a_horizontalDirection != 0 && a_horizontalDirection != 1 && a_horizontalDirection != -1
        && a_verticalDirection != 0 && a_verticalDirection != 1 && a_verticalDirection != -1 )
    {
        return;
    }
    
    // Direction:
    if (a_horizontalDirection == 1)
    {
        move(Right);
    }
    else if (a_horizontalDirection == -1)
    {
        move(Left);
    }
    else if (a_verticalDirection == 1)
    {
        move(Down);
    }
    else if (a_verticalDirection == -1)
    {
        move(Up);
    }
}

void Character::draw(void) const
{
    GRAPHICS.draw(m_animation, m_x, m_y);
}
