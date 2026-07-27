// Include:
#include "PhysicsEntity.h"
#include "../../Graphics/GraphicsSystem.h"
#include "../../Simulation/WorldSystem.h"

// returns true if either collision check resulted in a collision
bool PhysicsEntity::collisionScript()
{
    if (collisionWithWorldEntities() != nullptr || collisionWithMap() == true)
    {
        return true;
    }

    return false;
}

// loops through all the worlds entities looking for a potential collision,
// if a collision is found with a collision mask that is not detection only,
// then the entity will be forcefully put at the edge of the collided entity
// returns the entity that was collided with or nullptr
const Entity* PhysicsEntity::collisionWithWorldEntities(void)
{
    // Entities:
    const std::list<Entity*>* worldEntities = &WORLD_SYSTEM.getEntities();
    constexpr int spriteSize = SPRITE_SIZE;

    // Loop through collisions:
    for (const Entity* currentEntity : *worldEntities)
    {
        // Filter Entities We Don't intend to collide with:
        if (currentEntity == this || currentEntity == m_owner)
        {
            continue;
        }

        // Set Current Collision Mask
        const CollisionMask* currentCollision = &currentEntity->getCollisionMask();

        // Check For Collision:
        if (m_collision.intersection(*currentCollision) == true)
        {
            // filter out detection only
            if (currentCollision->getDetectionOnly() == true)
            {
                continue;
            }

            // Using Velocity as Direction:
            // Teleport to edge of collision:
            const MyVector collisionPosition = currentCollision->getPosition();
            constexpr double epsilon = 1e-6;

            if (std::abs(m_velocity.m_x) > epsilon)
            {
                m_position.m_x = collisionPosition.m_x + spriteSize * ((m_velocity.m_x < 0) - (m_velocity.m_x > 0));
                m_velocity.setVector(0, 0);
            }
            else if (std::abs(m_velocity.m_y) > epsilon)
            {
                m_position.m_y = collisionPosition.m_y + spriteSize * ((m_velocity.m_y < 0) - (m_velocity.m_y > 0));
                m_velocity.setVector(0, 0);
            }
            else
            {
                m_position.m_x = collisionPosition.m_x - spriteSize;
                m_velocity.setVector(0, 0);
            }

            return currentEntity;
        }
    }

    return nullptr;
}

// Gets the surrounding tiles around the entity and checks if 
// the entity will collide with them and if so does that surrounding,
// tile have collision.
// returns a boolean for if there was a collision or not
bool PhysicsEntity::collisionWithMap(void)
{
    // Saving variables for performance:
    constexpr int spriteSize = SPRITE_SIZE;
    constexpr int tileColumns = TILE_COLlUMNS;

    // Check if we are inside the bounds of the world
    if (m_position.m_x + spriteSize < 0 || m_position.m_x > WORLD_WIDTH
        || m_position.m_y + spriteSize < 0 || m_position.m_y > WORLD_HEIGHT)
    {
        return false;
    }

    // Map:
    // Get estimate of position to tile index
    const float tileX = std::round(m_position.m_x / spriteSize);
    const float tileY = std::round(m_position.m_y / spriteSize);

    // defining surrounding tiles (all at once to avoid using push_back which has to re-size and add value to the vector one after another)
    // (note : also means surround tile indexes can be const) : ICA
    const std::vector<int> surroundTileIndexes{
        static_cast<int>(tileY * tileColumns + tileX),              // Current Tile
        static_cast<int>(tileY * tileColumns + (tileX + 1)),        // Tile to the right
        static_cast<int>(tileY * tileColumns + (tileX - 1)),        // Tile to the left
        static_cast<int>((tileY + 1) * tileColumns + tileX),        // Tile below
        static_cast<int>((tileY - 1) * tileColumns + tileX),        // Tile Above
        static_cast<int>((tileY - 1) * tileColumns + (tileX - 1)),  // Top Left
        static_cast<int>((tileY - 1) * tileColumns + (tileX + 1)),  // Top Right
        static_cast<int>((tileY + 1) * tileColumns + (tileX - 1)),  // Bottom Left
        static_cast<int>((tileY + 1) * tileColumns + (tileX + 1))   // Bottom Right
    };

    // Loop through surrounding tiles for collisions:
    const std::vector<Tile>& tiles = WORLD_SYSTEM.getMap();
    const int amountOfTiles = static_cast<int>(tiles.size());
    for (const int& currentTileIndex : surroundTileIndexes)
    {
        if (currentTileIndex < 0 || currentTileIndex >= amountOfTiles)
        {
            continue;
        }

        // filter out non collides:
        if (tiles[currentTileIndex].m_tileCollidable == false)
        {
            continue;
        }

        // Calculating Tile Position:
        const int tileColumnIndex = currentTileIndex % tileColumns;
        const int tileRowIndex = currentTileIndex / tileColumns;

        // Tile Bounds in world:
        const float tileLeft = static_cast<float>(tileColumnIndex * spriteSize);
        const float tileTop = static_cast<float>(tileRowIndex * spriteSize);
        const float tileRight = static_cast<float>((tileColumnIndex + 1) * spriteSize);
        const float tileBottom = static_cast<float>((tileRowIndex + 1) * spriteSize);

        // Collision check using bounds:
        if (m_collision.intersection(tileLeft, tileTop, tileRight, tileBottom) == true)
        {
            // Using Velocity as Direction:
            // Teleport to edge of collision:
            if (std::abs(m_velocity.m_x) > 0)
            {
                m_position.m_x = tileLeft + SPRITE_SIZE * ((m_velocity.m_x < 0) - (m_velocity.m_x > 0));
                m_velocity.setVector(0, 0);
            }
            else if (std::abs(m_velocity.m_y) > 0)
            {
                m_position.m_y = tileTop + SPRITE_SIZE * ((m_velocity.m_y < 0) - (m_velocity.m_y > 0));
                m_velocity.setVector(0, 0);
            }
            else
            {
                m_position.m_x = tileLeft - SPRITE_SIZE;
                m_velocity.setVector(0, 0);
            }

            return true;
        }
    }

    return false;
}

// simple move function that checks for collisions and updates entities position and mask
void PhysicsEntity::move(const MyVector& a_force, const double& a_deltaTime)
{
    m_velocity = a_force;

    // Collisions:
    m_collision.setPosition(m_position + m_velocity + m_collisionMaskOffset);
    collisionScript();

    // Updating Positions:
    m_position = m_position + m_velocity;
    m_collision.setPosition(m_position + m_collisionMaskOffset);
}
