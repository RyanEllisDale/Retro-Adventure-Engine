// Include:
#include "Player.h"
#include "projectile.h"
#include "../Sound/SoundSystem.h"
#include "../Simulation/WorldSystem.h"


// Construction:
Player::Player()
{
    // Physics:
    m_massG = 70000;
    m_dragCoefficient = 0.83f;
    m_movementForce = 88000.0f;
    m_collision.setPosition(m_position);

    // Animations:
    const std::string spritesheet = "Data/Textures/LinkSpritesheet.png";
    const std::vector<MyVector> walkDownCoordinates = {{2,22},{36,22}};
    const std::vector<MyVector>  walkUpCoordinates = {{138,22},{172,22}};
    const std::vector<MyVector>  walkRightCoordinates = {{70,22},{104,22}};

    m_walkDown = Animation(spritesheet,walkDownCoordinates,2,true);
    m_walkUp = Animation(spritesheet,walkUpCoordinates,2,true);
    m_walkSide = Animation(spritesheet,walkRightCoordinates,2,true);
    m_animation = &m_walkDown;

    // Persistence:
    m_isPersistent = true;
}

// simple update loop, most functionality happens in overwrited updateInput and die
// moves the player stopping for collisions with world tile map and other entities:
void Player::update(const double& a_deltaTime, const float& a_simulationTimeScalar)
{
    // Movement:
    move(m_velocity, a_deltaTime);

    // Animation:
    m_animation->update();
    draw(a_simulationTimeScalar);
  
}

// handles player action,
// arrow keys / WASD moves the player,
// Select (Backspace) sets the difficulty to normal,
// Start (Enter) sets the difficulty to hard,
// And button 1 (Z) shoots a projectile
void Player::updateInput(const std::vector<Input>& a_inputs)
{
    // Remove Movement Force:
    m_velocity.setVector(0.0f, 0.0f);

    // loop through all inputs:
    for (const Input& currentInput : a_inputs)
    {
        // Using If Statement For Optional Break Statement

        // Movement:
        // determines sprite flip for entity, what animation to play and the velocity of the player
        if (currentInput == Input::Left)
        {
            m_animation = &m_walkSide;
            m_xFlip = true;

            MyVector movementForce = MyVector(-m_movementForce,0.0f);
            m_velocity = Physics::calculateAcceleration(movementForce,m_massG,m_dragCoefficient,Physics::frictionCoefficient);
        }
        else if (currentInput == Input::Right)
        {
            m_animation = &m_walkSide;
            m_xFlip = false;

            MyVector movementForce = MyVector(m_movementForce,0.0f);
            m_velocity = Physics::calculateAcceleration(movementForce,m_massG,m_dragCoefficient,Physics::frictionCoefficient);
        }
        else if (currentInput == Input::Up)
        {
            m_animation = &m_walkUp;
            m_xFlip = false;

            MyVector movementForce = MyVector(0.0,-m_movementForce);
            m_velocity = Physics::calculateAcceleration(movementForce,m_massG,m_dragCoefficient,Physics::frictionCoefficient);
        }
        else if (currentInput == Input::Down)
        {
            m_animation = &m_walkDown;
            m_xFlip = false;
            
            const MyVector movementForce = MyVector(0.0,m_movementForce);
            m_velocity = Physics::calculateAcceleration(movementForce,m_massG,m_dragCoefficient,Physics::frictionCoefficient);
        }

        // Shooting:
        // checks if player has the sword, and isn't on cooldown,
        // Gets the players current animation and uses it as a direction,
        // spawns new sword
        else if (currentInput == Input::Button1)
        {
            if (m_hasSword == false)
            {
                continue;
            }
            
            if (m_swordClock.getElapsedTime().asSeconds() < m_swordCooldown.asSeconds())
            {
                continue;
            }
            
            m_swordClock.restart(); // reset cooldown
            MyVector swordInitialVelocityForce;
            
            // Spawning fireball based on direction
            if (m_animation == &m_walkSide)
            {
                if (m_xFlip == true) { swordInitialVelocityForce = MyVector{-m_swordShootingForce,0.0f}; }
                else { swordInitialVelocityForce = MyVector{m_swordShootingForce,0.0f}; }
            }
            else if (m_animation == &m_walkDown)
            {
                swordInitialVelocityForce = MyVector(0.0f,m_swordShootingForce);
            }
            else if (m_animation == &m_walkUp)
            {
                swordInitialVelocityForce = MyVector(0.0f,-m_swordShootingForce);
            }

            Entity* ptr = this; // Used to set swords owner
            new Projectile(m_position,ptr,swordInitialVelocityForce);
        }

        // Game Difficulty:
        else if (currentInput == Input::Select)
        {
            WORLD_SYSTEM.m_difficulty = normal;
        }
        else if (currentInput == Input::Start)
        {
            WORLD_SYSTEM.m_difficulty = hard;
        }

    }
}

// Restarts the game,
// plays the game over sound, 
// then calls the game over function which deletes the player, re-builds them then loads level 1, 
// cycles the game
void Player::Die(void)
{
    const std::string gameOverSoundFile = "Data/Sounds/gameOver.wav";
    SOUND_SYSTEM.playSound(gameOverSoundFile, 3);

    WORLD_SYSTEM.gameOver();
}
