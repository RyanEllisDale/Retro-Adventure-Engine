// Include:
#include "CopterEnemy.h"
#include "projectile.h"
#include "../../Simulation/WorldSystem.h"

CopterEnemy::CopterEnemy()
{
    // Physics:
    m_collision.setPosition(m_position);

    // difficulty:
    // sets the pallete, health and shooting cooldown of the copter
    if (WorldSystem::getDifficulty() == hard)
    {
        m_maxHealth = 3;
        m_pallete = 2;
        m_shootTime = 1.5f;
    }
    else
    {
        m_maxHealth = 2;
        m_pallete = 1;
    }

    // Animations:
    const std::string spritesheet = "Data/Textures/overworldEnemies.png";
    const std::vector<MyVector> idleCoordinates = { {324,118},{358,118} };
    m_idle = Animation(spritesheet, idleCoordinates, 2, true);
    m_animation = &m_idle;
}

// checks if the timer for shooting more swords is up,
// if so shoots out for swords in the 4 cardinal directions
void CopterEnemy::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    // shooting:
    if (m_shootClock.getElapsedTime().asSeconds() > m_shootTime)
    {
        m_shootClock.restart();

        // spawning and shooting projectiles:
        Entity* ptr = this; // Used to set projectiles owner
        new Projectile(m_position, ptr, MyVector(-m_projectileForce, 0.0f));
        new Projectile(m_position, ptr, MyVector(m_projectileForce, 0.0f));
        new Projectile(m_position, ptr, MyVector(0.0f, m_projectileForce));
        new Projectile(m_position, ptr, MyVector(0.0f, -m_projectileForce));
    }

    // visuals:
    m_animation->update();
    draw(a_simulationTimeScalar);
}










