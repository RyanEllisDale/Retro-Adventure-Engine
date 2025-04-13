// Small simple physics class to be expanded upon
// fully static so all physics calculations can be done without an instance of physics
// Should not be created

#pragma once

// Includes:
#include "MyVector.h"

class Physics
{
private:
    Physics() = delete;

public:
    // Constant World Physics Members:
    static constexpr float gravitationalConst = 9.81f;
    static constexpr float frictionCoefficient = 0.2f;
    
    // Physics Calculations:
    static const float calculateWeight(const float& a_massG);
    static const MyVector calculateAcceleration(const MyVector& a_force, const float& a_massG,
        const float& a_windResistance, const float& a_friction);
    static const MyVector applyDeceleration(MyVector& a_velocity, float a_deceleration, float a_deltaTime);
};


inline const float Physics::calculateWeight(const float& a_massG)
{
    return (a_massG/1000) * gravitationalConst;
}

inline const MyVector Physics::calculateAcceleration(const MyVector& a_force, const float& a_massG,
    const float& a_windResistance = 0.0f, const float& a_friction = 0.0f)
{
    // Saving force vars for manipulation:
    // Calculating Net Force:
    float forceX = a_force.m_x;
    float forceY = a_force.m_y;

    // Calculate sign bit (-1 for negatives, 1 for positives),
    // used to revert calculated acceleration back to the initial direction of a_force
    const int signX = forceX > 0 ? 1 : -1;
    const int signY = forceY > 0 ? 1 : -1;
    
    // If force is not 0 then calculate absolute value of force
    // for both x and y
    if (std::fabs(0 - forceX) > 0.001f)
    {
        forceX = std::fabs(forceX) - a_windResistance - a_friction;
    }
    if (std::fabs(0 - forceY) > 0.001f)
    {
        forceY = std::fabs(forceY) - a_windResistance - a_friction;
    }

    // Calculating Acceleration
    const MyVector netForce(forceX,forceY); 
    MyVector acceleration = netForce / a_massG;
    acceleration.m_x = acceleration.m_x * signX; // return direction to original x direction
    acceleration.m_y = acceleration.m_y * signY; // return direction to original y direction
    
    return acceleration;
}


inline const MyVector Physics::applyDeceleration(MyVector& a_velocity, const float a_deceleration, const float a_deltaTime)
{
    // If the velocity is near zero then stop velocity
    // (magnitude is always positive)
    if (a_velocity.getMagnitude() < 0.0001f)
    {
        a_velocity = {0, 0};
        return a_velocity;
    }

    // new decleration
    // if deceleration is near zero then set velocity to zero
    const MyVector decelerationVector = (a_velocity * a_deceleration) * a_deltaTime;
    if (decelerationVector.getMagnitude() > a_velocity.getMagnitude())
    {
        a_velocity = {0, 0};
        return a_velocity;
    }
    // subtract deceleration from main / current velocity
    else
    {
        a_velocity = a_velocity - decelerationVector;
        return a_velocity;
    }
}
