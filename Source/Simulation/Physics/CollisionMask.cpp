// Include
#include "CollisionMask.h"
#include "../../Graphics/GraphicsSystem.h"

CollisionMask::CollisionMask(const float a_x, const float a_y, const bool a_detectionOnly,
    int a_width, int a_height) : m_position(a_x,a_y), m_detectionOnly(a_detectionOnly)
{
    // default size if size not specified
    if (static_cast<int>(a_width) == -999 && static_cast<int>(a_height) == -999)
    {
        a_width = SPRITE_SIZE;
        a_height = SPRITE_SIZE;
    }

    m_width = a_width;
    m_height = a_height;
}

CollisionMask::CollisionMask(const MyVector& a_position, const bool a_detectionOnly,
    int a_width, int a_height) : m_position(a_position), m_detectionOnly(a_detectionOnly)
{
    // default size if size not specified
    if (a_width == -999 && a_height == -999)
    {
        a_width = SPRITE_SIZE;
        a_height = SPRITE_SIZE;
    }

    m_width = a_width;
    m_height = a_height;
}

// Intersection:
// Master function, calculates if a list of floats representing two rectangles are in each others bounds.
bool CollisionMask::intersection(const float& a_left1, const float& a_top1, const float& a_right1, const float& a_bottom1, const float& a_left2, const float& a_top2, const float& a_right2, const float& a_bottom2)
{
    if (a_left1 >= a_right2 // Left Bigger Than other right position
        || a_top1 >= a_bottom2 // Top Bigger Than Bottom position
        || a_right1 <= a_left2 // Right less than left position
        || a_bottom1 <= a_top2) //bottom less than top position
    {
        return false;
    }

    return true;
}

// Functions based off master function:
bool CollisionMask::intersection(const CollisionMask& a_rectangle) const
{
    return CollisionMask::intersection(m_position.m_x, m_position.m_y,
        m_position.m_x + m_width, m_position.m_y + m_height,
        a_rectangle.m_position.m_x, a_rectangle.m_position.m_y,
        a_rectangle.m_position.m_x + a_rectangle.m_width, a_rectangle.m_position.m_y + a_rectangle.m_height);
}

bool CollisionMask::intersection(const CollisionMask* a_rectangle) const
{
    return intersection(*a_rectangle);
}

bool CollisionMask::intersection(const float& a_left, const float& a_top, const float& a_right, const float& a_bottom) const
{
    return CollisionMask::intersection(m_position.m_x,m_position.m_y,m_position.m_x + m_width,m_position.m_y + m_height,
        a_left, a_top, a_right, a_bottom);  
}

// Position:
// returning const reference so it can't be edited when called by this function:
// syntax function
const MyVector& CollisionMask::getPosition(void) const
{
    return m_position;
}

// syntax function
void CollisionMask::setPosition(const MyVector& a_position)
{
    m_position = a_position;
}

const bool& CollisionMask::getDetectionOnly(void) const
{
    return m_detectionOnly;
}
