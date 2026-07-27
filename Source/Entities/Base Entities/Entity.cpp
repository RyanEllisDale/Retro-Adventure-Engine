// Include:
#include "Entity.h"
#include <algorithm>
#include "../../Graphics/GraphicsSystem.h"
#include "../../Simulation/WorldSystem.h"

// Construction:
// Adding And Removing Entity From World:
Entity::Entity()
{
    WORLD_SYSTEM.addEntity(this);
}

void Entity::destroy()
{
    WORLD_SYSTEM.removeEntity(this);
}

void Entity::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    draw(a_simulationTimeScalar);
}

// Member Functions:
void Entity::draw(const float& a_simulationTimeScalar)
{
    // lerp position:
    MyVector lerpedPosition = m_oldPosition + ((m_position - m_oldPosition) * a_simulationTimeScalar);
    m_oldPosition = m_position;

    if (m_animation != nullptr) { m_animation->draw(lerpedPosition,m_pallete,m_xFlip,m_yFlip); }
}

// Getters:
const CollisionMask& Entity::getCollisionMask() const
{
    return m_collision;
}

const bool& Entity::getIsPersistent() const
{
    return m_isPersistent;
}

// Setters:
void Entity::setPosition(MyVector a_position)
{ 
    m_position = a_position; 
    m_oldPosition = m_position;
    m_collision.setPosition(m_position + m_collisionMaskOffset); 
}

