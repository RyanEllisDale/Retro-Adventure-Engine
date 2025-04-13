// simple lightweight vector class (8 bytes)
// part of physics so has static functions for vector maths (without needing a vector)
// highly convertable to sfml
// used throughout most of the engine

//Completely public class to be edited by everything that needs it

#pragma once

// Includes:
#include "../SFML/include/SFML/System/Vector2.hpp" // for conversions
#include <cmath> // for float absolute function

// Completely open vector physics class
// Conversion operators
class MyVector
{
public:
    // Coords:
    float m_x{0};
    float m_y{0};
    
    // Construction:
    MyVector(void) = default;
    MyVector(const int a_x, const int a_y) : m_x(static_cast<float>(a_x)), m_y(static_cast<float>(a_y)) {}
    MyVector(const float a_x, const float a_y) : m_x(a_x), m_y(a_y) {}

    // Conversion Constructors:
    MyVector(const sf::Vector2f& a_other) : m_x(a_other.x), m_y(a_other.y) {}
    MyVector(const sf::Vector2i& a_other) : m_x(static_cast<float>(a_other.x)), m_y(static_cast<float>(a_other.y)) {}
    MyVector(const sf::Vector2u& a_other) : m_x(static_cast<float>(a_other.x)), m_y(static_cast<float>(a_other.y)) {}
    
    // Static Functions:
    static const float calculateMagnitude(const MyVector& a_vector);
    static const float calculateMagnitudeSquared(const MyVector& a_vector);
    static const float calculateDotProduct(const MyVector& a_leftVector, const MyVector& a_rightVector);
    static const float calculateCrossProduct(const MyVector& a_leftVector, const MyVector& a_rightVector);
    static const float calculateSum(const MyVector& a_vector);
    
    // Set Vector
    MyVector& setVector(const float& a_x, const float& a_y);
    MyVector& normaliseVector(void);
    
    // Get Calculated Vars For Physics:
    // Not const so they can be editable and manipulated for maths
    auto getMagnitude(void) const -> float; 
    auto getMagnitudeSquared(void) const -> float;
    auto getDotProduct(const MyVector& a_anotherVector) const -> float;
    auto getCrossProduct(const MyVector& a_anotherVector) const -> float;
    auto getSum(void) const -> float;
    
    // operator overloading:
    #pragma region Overloading Operators 
    
    // Conversions to other types:
    operator sf::Vector2f() const { return {m_x,m_y}; }
    operator sf::Vector2i() const { return {static_cast<int>(m_x), static_cast<int>(m_y)}; }
    operator sf::Vector2u() const { return {static_cast<unsigned int>(m_x), static_cast<unsigned int>(m_y)}; }

    // Boolean Operations:

    // Calculate difference between x's and y's then check if its minimal (basically 0)
    // if the difference is negligible then == is true 
    const bool operator==(const MyVector& a_otherVector) const
    {
        // Can't compare floats directly (== is unsafe)
        if ((std::fabs(m_x - a_otherVector.m_x) < 0.001) &&
            (std::fabs(m_y - a_otherVector.m_y) < 0.001))
        {
            return true;
        }
    
        return false;
    }

    const bool operator!=(const MyVector& a_otherVector) const { return !(*this == a_otherVector); }
    const bool operator<(const MyVector& a_otherVector) const { return getSum() < a_otherVector.getSum(); }
    const bool operator>(const MyVector& a_otherVector) const { return getSum() > a_otherVector.getSum(); }

    // Athrimatic Operators:
    
    // Makes new vectors and returns those
    // const operators
    MyVector operator+(const MyVector& a_otherVector) const { return MyVector(m_x + a_otherVector.m_x,m_y + a_otherVector.m_y); }
    MyVector operator-(const MyVector& a_otherVector) const { return MyVector(m_x - a_otherVector.m_x,m_y - a_otherVector.m_y); }
    MyVector operator*(const MyVector& a_otherVector) const { return MyVector(m_x * a_otherVector.m_x,m_y * a_otherVector.m_y); }
    MyVector operator*(const float& a_scalar) const { return MyVector(m_x * a_scalar,m_y * a_scalar); }
    MyVector operator/(const MyVector& a_otherVector) const { return MyVector(m_x / a_otherVector.m_x,m_y / a_otherVector.m_y); }
    MyVector operator/(const float& a_scalar) const { return MyVector(m_x / a_scalar,m_y / a_scalar); }
    
    //Updates this vector and returns itself
    MyVector operator+=(const MyVector& a_otherVector)
    {
        m_x = m_x + a_otherVector.m_x;
        m_y = m_y + a_otherVector.m_y;
        return *this;
    }
    MyVector operator-=(const MyVector& a_otherVector)
    {
        m_x = m_x - a_otherVector.m_x;
        m_y = m_y - a_otherVector.m_y;
        return *this;
    }
    
    #pragma endregion Overloading Operators  
};


