// Includes:
#include "Sword.h"
#include "Player.h"
#include <typeinfo>
#include "../Simulation/WorldSystem.h"

// construction:
Sword::Sword()
{
    // Collision
    // sets the collision mask to detect only because sword does not want any,
    // physics related collisions,
    m_collision = CollisionMask(m_position, true);
     
    // Animation:
    // static image
    const std::string filepath = "Data/Textures/LinkSpritesheet.png";
    const std::vector<MyVector> textureCoordinates = { {2,308} };
    m_idle = Animation(filepath, textureCoordinates, 0, false);
    m_animation = &m_idle;
}

// checks for collisions with the player,
// if one has occured then it will destroy itself
void Sword::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    if (collisionScript() == true)
    {
        destroy();
        return;
    }

    draw(a_simulationTimeScalar);
}

// loops through all the worlds entities to find the player
// when finding the player it checks for a collision, 
// if they are colliding, it will give the player a sword before returning true
bool Sword::collisionScript(void)
{
    // Entities:
    const std::list<Entity*>* worldEntities = &WORLD_SYSTEM.getEntities();

    // Loop through collisions:
    for (Entity* currentEntity : *worldEntities)
    {
        // filter out none players
        Player* entity = dynamic_cast<Player*>(currentEntity);
        if (entity == nullptr)
        {
            continue;
        }

        // Set Current Collision Mask
        const CollisionMask* currentCollision = &entity->getCollisionMask();

        // Check For Collision:
        if (m_collision.intersection(currentCollision) == true)
        {
            entity->m_hasSword = true;
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
