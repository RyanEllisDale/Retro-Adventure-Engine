// Includes:
#include "projectile.h"
#include "../../Simulation/WorldSystem.h"
#include "../../Sound/SoundSystem.h"
#include "../Graphics/GraphicsSystem.h"
#include "Base Entities/Character.h"

// Construction:
// Setting Up sword:
Projectile::Projectile(const MyVector& a_position, Entity*& a_owner, const MyVector& a_velocity)
{
    m_owner = a_owner; // prevents projectile from damaging spawning entity
    m_position = a_position; 

    // Physics:
    m_massG = 40.0f;
    m_dragCoefficient = 0.99f;
    m_velocity = Physics::calculateAcceleration(a_velocity,m_massG,m_dragCoefficient); // initial force:
    
    // Animations:
    const std::string filepath = "Data/Textures/LinkSpritesheet.png";
    m_animation = &m_horizontal;

    std::vector<MyVector>  textureCoordinates = {{2,308}};
    m_vertical = Animation(filepath, textureCoordinates, 0, false);
    textureCoordinates[0].setVector(36,308);
    m_horizontal = Animation(filepath,textureCoordinates,0,false);

    // collision mask:
    // default to vertical collision for vertical sprite
    m_collisionMaskOffset = MyVector(0.0f, 8.0f);
    int collisionWidth = 16;
    int collisionHeight = 32;

    // determining visuals and collisions based on velocity direction:
    if (m_velocity.m_x != 0)
    {
        if (m_velocity.m_x < 0) 
        {
            m_xFlip = true; 
        }
        else 
        { 
            m_xFlip = false; 
        }

        // Switch to horizontal collisions
        m_collisionMaskOffset.setVector(0.0f, 8.0f);
        collisionWidth = 32;
        collisionHeight = 16;
    }
    else if (m_velocity.m_y != 0)
    {
        m_animation = &m_vertical;
        if (m_velocity.m_y < 0) 
        { 
            m_yFlip = false;
        }
        else 
        { 
            m_yFlip = true; 
        }
    }

    m_collision = CollisionMask(a_position + m_collisionMaskOffset, true, collisionWidth, collisionHeight);

    // Sound:
    SOUND_SYSTEM.playSound("Sounds/fire.wav", 0);
}

// gradually slows down projectile over time, 
// keeps travelling until object hits something or slows down to the point of death
void Projectile::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    // Moving:
    Physics::applyDeceleration(m_velocity, 0.01f, 1);
    move(m_velocity, a_deltaTime);

    // Animation:
    m_animation->update();
    draw(a_simulationTimeScalar);
}

// moves the projectile an ever decreasing amount,
// destroys projectile once projectile slows down to less than 0.1 force
void Projectile::move(const MyVector& a_movementVector, const double& a_deltaTime)
{
    // Checking if we have slowed down to the point of death:
    if (((std::fabs(0 - m_velocity.m_x)) <= 0.1) &&
        ((std::fabs(0 - m_velocity.m_y)) <= 0.1))
    {
        destroy();
        return;
    }

    // Moving:
    // Collisions:
    m_collision.setPosition(m_position + m_velocity);
    if (collisionScript() == true)
    {
         destroy();
    }
    
    // Updating Positions:
    m_position = m_position + m_velocity;
    m_collision.setPosition(m_position + m_collisionMaskOffset);
}

// custom collision to deal damage to characters:
const Entity* Projectile::collisionWithWorldEntities(void)
{
    // Entities:
    const std::list<Entity*>* worldEntities = &WORLD_SYSTEM.getEntities();
    constexpr int spriteSize = SPRITE_SIZE;

    // Loop through collisions:
    for (Entity* currentEntity : *worldEntities)
    {
        // Filter Entities We Don't intend to collide with:
        if (currentEntity == this || currentEntity == m_owner)
        {
            continue;
        }

        // Set Current Collision Mask
        const CollisionMask* currentCollision = &currentEntity->getCollisionMask();

        // Check For Collision:
        if (m_collision.intersection(currentCollision) == true)
        {
            const Entity* ptr = currentEntity;
            Character* charPtr = dynamic_cast<Character*>(currentEntity);
            if (charPtr == nullptr)
            {
                return nullptr;
            }

            // dealing damage
            charPtr->takeDamage(m_damage);
            return ptr;
        }
    }

    return nullptr;
}
