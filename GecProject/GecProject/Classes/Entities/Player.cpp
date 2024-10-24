// Include:
#include "Player.h"

// Construction:
Player::Player(const unsigned short& a_x, const unsigned short& a_y)
{
    m_health = 3;
    m_movementSpeed = 0.2;
    
    // Position:
    m_x = a_x;
    m_y = a_y;
    
    // Animation Building:
    // Path:
    const std::string mileStone1TextureFilePath = "Data/Textures/LinkSpriteSheet.png";

    m_walkDown.m_name = "Walk Down";
    m_walkDown.m_spriteSheetFilePath = mileStone1TextureFilePath;
    m_walkDown.m_frameCoords = std::vector<TextureCoords> {TextureCoords(4,44),TextureCoords(72,44)};

    m_walkUp.m_name = "Walk Up";
    m_walkUp.m_spriteSheetFilePath = mileStone1TextureFilePath;
    m_walkUp.m_frameCoords = std::vector<TextureCoords> {TextureCoords(276,44),TextureCoords(344,44)};

    m_walkSide.m_name = "Walk Side";
    m_walkSide.m_spriteSheetFilePath = mileStone1TextureFilePath;
    m_walkSide.m_frameCoords = std::vector<TextureCoords> {TextureCoords(140,44),TextureCoords(208,44)};
    
    m_animation.setAnimation(m_walkDown);
}

// Functions:
void Player::update()
{
    // Direction / Input:
    Direction movingDirection = Null;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movingDirection = Left;
        m_animation.setAnimation(m_walkSide);
        m_animation.setScale(-1,1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movingDirection = Right;
        m_animation.setAnimation(m_walkSide);
        m_animation.setScale(1,1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movingDirection = Up;
        m_animation.setAnimation(m_walkUp);
        m_animation.setScale(1,1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movingDirection = Down;
        m_animation.setAnimation(m_walkDown);
        m_animation.setScale(1,1);
    }

    // Movement
    if (movingDirection != Null)
    {
        move(movingDirection);
    }

    m_animation.update();
}

void Player::draw() const
{
    Character::draw();
}
