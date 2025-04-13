// Entity: (Root of entities)
// All entities are cast up to this class when updated in world:
// can draw itself to the graphics system and can be updated by the world system
// basis for game objects:

// Config:
#pragma once

// Include:
#include "../../Simulation/Physics/CollisionMask.h"
#include "../../Graphics/Animation.h"

// forward declare owner world
class WorldSystem;

// Stationary Static Minimal Entity Class:
class Entity
{
private:
    friend class WorldSystem;
    
    // Private Functionality:
    // non-changing const functions, callable from cast (Entity*())
    void setPosition(MyVector a_position);
    const MyVector& getPosition(void) const { return m_position; }
    const bool& getIsPersistent(void) const;

protected:
    Entity* m_owner{ nullptr };
    Animation* m_animation{nullptr};
    CollisionMask m_collision{ 0.0f,0.0f };
    MyVector m_oldPosition{ 0,0 };
    MyVector m_position{0,0};
    MyVector m_collisionMaskOffset{ 0.0f,0.0f };

    // Flip: Entity Scale
    bool m_xFlip{false};
    bool m_yFlip{false};
    bool m_isPersistent{false};
    uint8_t m_pallete{0};
    
    // callable protected functions:
    void destroy(void);


public:
    // Construction:
    Entity(void);
    Entity(const Entity& a_other) = delete;
    virtual ~Entity(void) = default;
    
    // Member Functions:
    virtual void update(const double& a_deltaTime, const float& a_simulationTimeScalar);
    void draw(const float& a_simulationTimeScalar);
    const CollisionMask& getCollisionMask(void) const;
  
};
