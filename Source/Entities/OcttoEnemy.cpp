// Include:
#include "OcttoEnemy.h"
#include "projectile.h"
#include "../../Simulation/WorldSystem.h"

// Construction:
OcttoEnemy::OcttoEnemy()
{
    // Physics:
    m_massG = 70000;
    m_dragCoefficient = 0.83f;
    m_collision.setPosition(m_position);
    
    // difficulty:
    // determines pallete and health for difficulty:
    m_maxHealth = 1;
    if (WorldSystem::getDifficulty() == hard)
    {
        m_maxHealth = 2;
        m_shootTime = 2.5f;
    }
    m_currentHealth = m_maxHealth;
    m_pallete = m_maxHealth;

    // Animations:
    const std::string spritesheet = "Data/Textures/overworldEnemies.png";
    const std::vector<MyVector> walkDownCoordinates = { {2,22},{36,22} };
    const std::vector<MyVector>  walkSideCoordinates = { {70,22},{104,22} };

    m_walkDown = Animation(spritesheet, walkDownCoordinates, 2, true);
    m_walkSide = Animation(spritesheet, walkSideCoordinates, 2, true);
    m_animation = &m_walkDown;

    pickDirection();
}

// randomly picks a direction,
// first determines if it will be a horizontal or vertical direction,
// then determines if it will be positive or negative along that axis
void OcttoEnemy::pickDirection(void)
{
    int axis = rand() % 2;
    int xDirection = 0;
    int yDirection = 0;

    // determining what axis it will be and then:
    // generates a random direction in that axis (postitive or negative),
    // sets the flip data off of that direction,
    // sets the animation based of the axis

    if (axis == 0) // horizontal
    {
        xDirection = rand() % 2;
        xDirection = (xDirection == 0) ? xDirection = -1 : xDirection = 1;
        m_xFlip = (xDirection == 1) ? m_xFlip = true : m_xFlip = false;
        m_yFlip = false;
        m_animation = &m_walkSide;
    }
    else // vertical
    {
        yDirection = rand() % 2;
        yDirection = (yDirection == 0) ? yDirection = 1 : yDirection = -1;
        m_xFlip = false;
        m_yFlip = (yDirection == 1) ? m_yFlip = false : m_yFlip = true;
        m_animation = &m_walkDown;
    }

    // calculating velocity and movement force:
    m_velocity = MyVector(m_movementForce * xDirection, m_movementForce * yDirection);
    m_velocity = Physics::calculateAcceleration(m_velocity, m_massG, m_dragCoefficient, Physics::frictionCoefficient);
}

// when the Octto hits a wall it will automatically pick a different direction
bool OcttoEnemy::collisionWithMap(void)
{
    if (PhysicsEntity::collisionWithMap() == true)
    {
        pickDirection();
        return true;
    }

    return false;
}

// small finite state machine
void OcttoEnemy::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    // State
    switch (m_currentState)
    {
        // Checks timers to determine if it needs to change direction or change statem
        // then moves
        case m_moving:
        {
            // changing direction:
            if (m_movementClock.getElapsedTime().asSeconds() > m_movementTime)
            {
                pickDirection();
                m_movementClock.restart();
            }

            // changing state:
            // does not move if changing state to shooting:
            if (m_shootClock.getElapsedTime().asSeconds() > m_shootTime)
            {
                m_currentState = m_shooting;
                break;
            }

            move(m_velocity, a_deltaTime);
            break;
        }

        // stays still and spawns projectile after caclulating its initial velocity,
        // then reverts back to the moving state:
        case m_shooting:
        {

            MyVector swordInitialVelocityForce;

            // Spawning fireball based on direction
            if (m_animation == &m_walkSide)
            {
                if (m_xFlip == false) { swordInitialVelocityForce = MyVector{ -m_projectileForce,0.0f }; }
                else { swordInitialVelocityForce = MyVector{ m_projectileForce,0.0f }; }
            }
            else if (m_animation == &m_walkDown)
            { 
                if (m_yFlip == true) { swordInitialVelocityForce = MyVector(0.0f, -m_projectileForce); }
                else { swordInitialVelocityForce = MyVector(0.0f, m_projectileForce); }
            }

            Entity* ptr = this; // Used to set fireballs owner
            new Projectile(m_position, ptr, swordInitialVelocityForce);

            // revert back to moving:
            m_currentState = m_moving;
            m_movementClock.restart();
            m_shootClock.restart();

            break;
        }
    }

    // visuals:
    m_animation->update();
    draw(a_simulationTimeScalar);
}
