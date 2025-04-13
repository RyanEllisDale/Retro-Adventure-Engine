// Includes:
#include "MyVector.h"

// Calculations for vector maths and physics:
// Maths Calculations for 2d vector in 2d space:
#pragma region staticFunctions

const float MyVector::calculateMagnitude(const MyVector& a_vector)
{
    return std::sqrt((a_vector.m_x * a_vector.m_x) + (a_vector.m_y * a_vector.m_y));
}

const float MyVector::calculateMagnitudeSquared(const MyVector& a_vector)
{
    return (a_vector.m_x * a_vector.m_x) + (a_vector.m_y * a_vector.m_y);
}

const float MyVector::calculateDotProduct(const MyVector& a_leftVector, const MyVector& a_rightVector)
{
    return (a_leftVector.m_x * a_rightVector.m_x) + (a_leftVector.m_y * a_rightVector.m_y);
}

const float MyVector::calculateCrossProduct(const MyVector& a_leftVector, const MyVector& a_rightVector)
{
    return (a_leftVector.m_x * a_rightVector.m_x) - (a_leftVector.m_y * a_rightVector.m_y);
}

const float MyVector::calculateSum(const MyVector& a_vector)
{
    return a_vector.m_x + a_vector.m_y;
}

#pragma endregion


// Editing Vector:
// directly changing vector (mostly a syntax function since m_x and m_y are public)
MyVector& MyVector::setVector(const float& a_x, const float& a_y)
{
    m_x = a_x;
    m_y = a_y;
    return *this;
}

// normalising vector,
// changing its unit to one then returning itself for physics use and calculation
MyVector& MyVector::normaliseVector(void)
{
    // Comparing floating point with == or != is unsafe [clang-diagnostic-float-equal]
    // if (getMagnitude() == 0)
    if (std::fabs(0 - getMagnitude()) < 0.001)
    {
        m_x = 0;
        m_y = 0;
    }
    else
    {
        // avoid divide by 0
        m_x = m_x / getMagnitude();
        m_y = m_y / getMagnitude();
    }

    return *this;

}

// Getters, calculates maths for (this) vector from static functions then returns them 
// auto return types require -> float definition 
#pragma region getters

auto MyVector::getMagnitude(void) const -> float
{
    return calculateMagnitude(*this);
}

auto MyVector::getMagnitudeSquared(void) const -> float
{
    return calculateMagnitudeSquared(*this);
}

auto MyVector::getDotProduct(const MyVector& a_anotherVector) const-> float
{
    return calculateDotProduct(*this, a_anotherVector);
}

auto MyVector::getCrossProduct(const MyVector& a_anotherVector) const-> float
{
    return calculateCrossProduct(*this, a_anotherVector);
}

auto MyVector::getSum(void) const-> float
{
    return calculateSum(*this);
}

#pragma endregion