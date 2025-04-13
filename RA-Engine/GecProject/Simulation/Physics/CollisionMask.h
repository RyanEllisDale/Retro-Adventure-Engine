// collision masks that follow entities, 
// used to calculate intersections, contains a master intersection function,
// which then lets collision masks do intersection calculations on any arguments.

#pragma once

// Includes:
#include "MyVector.h"

// Class to be added to collide-able entities
// Determines when collisions are detected and what type of collision occur
class CollisionMask
{
private:
    // Private Members:
    MyVector m_position{ 0.0f,0.0f }; 

    // - 999 by default:
    // if it remains -999 then when the constructor is called the dimensions,
    // will default to the graphics defined SPRITE_SIZE; (standard universal size for program)
    int m_width{ -999 };
    int m_height{ -999 };

    bool m_detectionOnly{false}; // unchangeable

public:
    // Construction:
    CollisionMask(void) = default;
    ~CollisionMask(void) = default;
    CollisionMask(const float a_x, const float a_y, const bool a_detectionOnly = false,
        int a_width = -999, int a_height = -999);
    CollisionMask(const MyVector& a_position, const bool a_detectionOnly = false, int a_width = -999, int a_height = -999);

    // Intersection
    // master function
    static bool intersection(const float& a_left1, const float& a_top1, const float& a_right1, const float& a_bottom1,
        const float& a_left2, const float& a_top2, const float& a_right2, const float& a_bottom2);
    bool intersection(const CollisionMask& a_rectangle) const;
    bool intersection(const CollisionMask* a_rectangle) const;
    bool intersection(const float& a_left, const float& a_top, const float& a_right, const float& a_bottom) const;

    // Position
    const MyVector& getPosition(void) const;
    void setPosition(const MyVector& a_position);

    // Detection
    const bool& getDetectionOnly(void) const;
};
