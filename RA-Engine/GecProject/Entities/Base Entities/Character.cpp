// Include:
#include "Character.h"
#include "../../Simulation/WorldSystem.h"
#include "../../Graphics/GraphicsSystem.h"

// Member Functions:
 
// applies damage to health, 
// if health becomes zero, triggers character death
void Character::takeDamage(const int& a_damage)
{
    m_currentHealth = m_currentHealth - a_damage;
    if (m_currentHealth <= 0)
    {
        Die();
    }
}

// incriments the worlds score for the player,
// before calling the destroy function, queing the entity up,
// for deletion
void Character::Die(void)
{
    WORLD_SYSTEM.incrimentScore();
    destroy();
}
