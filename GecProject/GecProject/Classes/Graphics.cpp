// Includes:
#include "Graphics.h"
#include <iostream>

#pragma region AnimationClass

// Construction:
// Using Vector Of Frame Coords
Animation::Animation(const std::string& a_spriteSheetFilePath, const std::vector<TextureCoords>& a_frameCoords)
{
    m_animationData = AnimationData(a_spriteSheetFilePath,a_frameCoords);
}

Animation::Animation(AnimationData a_animationData) : m_animationData(a_animationData) {};

// Functions:
void Animation::update()
{
    // Progressing Animation:
    if (m_frameClock.getElapsedTime().asSeconds() >= 1.0 / ANIMATION_FRAME_RATE)
    {
        m_frameClock.restart();
        
        m_currentFrame = m_currentFrame + 1;
        
        // Loop Animation:
        if (m_currentFrame == m_animationData.m_frameCoords.size())
        {
            m_currentFrame = 0;
        }
    }
}

// Resets Animation Data
void Animation::resetAnimation(void)
{
    m_currentFrame = 0;
    m_frameClock.restart();
}

void Animation::setScale(const int a_xScale, const int a_yScale)
{
    if (a_xScale == 1 || a_xScale == -1)
    {
        m_xScale = a_xScale;
    }

    if (a_yScale == 1 || a_yScale == -1)
    {
        m_yScale = a_yScale;
    }
}

// Drawing:
void Animation::setAnimation(const struct ::AnimationData& a_animationData)
{
    if (a_animationData.m_name != m_animationData.m_name)
    {
        m_animationData = a_animationData;
        m_frameClock.restart();
    }
}

#pragma endregion AnimationClass
////////////////////////////////////////////////////////////

#pragma region GraphicsClass

// Graphics Class:
// Overriden Draw Event, Draws Loaded Sprite

// Singleton Instance:
Graphics* Graphics::m_instance = nullptr;
RenderWindow* Graphics::m_window = nullptr;

Graphics::Graphics()
{
    m_sprite.setOrigin(SPRITE_SIZE/2,SPRITE_SIZE/2);
}

void Graphics::createInstance(RenderWindow& a_window)
{
    // Error Handling:
    if (Graphics::m_instance == nullptr)
    {
        // Making Singleton instance:
        Graphics::m_instance = new Graphics;
    }

   Graphics::m_window = &a_window;
}

Graphics& Graphics::getInstance()
{
    return *m_instance;
}

void Graphics::destroyInstance()
{
    m_window = nullptr;
    m_instance = nullptr;
}

// Default Draw:
void Graphics::draw(sf::RenderTarget& a_target, const sf::RenderStates a_states) const
{
    a_target.draw(m_sprite, a_states);
}

// Full Draw Event,
// Takes Texture Coords And Sprite Sheet File Path
// Will Load Texture From File If Not Already Loaded
bool Graphics::draw(const std::string& a_spriteSheetFilePath,
     const unsigned short& a_leftTextureCoord, const unsigned short& a_topTextureCoord,
        const unsigned short& a_x, const unsigned short& a_y, const sf::RenderStates& a_states)
{
    // Loading New Texture:
    if (a_spriteSheetFilePath != m_currentSpriteSheetFilePath)
    {
        // Error Handling:
        // Loading Given File
        if (!m_texture.loadFromFile(a_spriteSheetFilePath))
        {
            // Loading Missing Texture
            std::cerr << "Failed to load sprite sheet \n";
           if(!m_texture.loadFromFile("Data/Textures/MissingTexture.png"))
           {
               // Creating Texture
               std::cerr << "Failed to load Missing Texture \n";
               if (!m_texture.create(200,200))
               {
                   std::cerr << "Failed to create texture \n";
                   return false;
               }

               const Uint8* pixels = new Uint8[200 * 200 * 4];
               m_texture.update(pixels);
           }
        }
        
        m_currentSpriteSheetFilePath = a_spriteSheetFilePath;
        m_sprite.setTexture(m_texture);
    }
    
    m_sprite.setPosition(a_x,a_y);
    m_sprite.setTextureRect(sf::IntRect(a_leftTextureCoord, a_topTextureCoord, SPRITE_SIZE, SPRITE_SIZE));

    m_window->draw(m_sprite, a_states);
    return true;
}

// Draw From Texture Coords Struct:
// Pulls Data From Given Texture Coords Struct And Passes To Full Draw Function:
void Graphics::draw(const std::string& a_spriteSheetFilePath,
    const TextureCoords& a_textureCoords, const unsigned short& a_x,
        const unsigned short& a_y, const sf::RenderStates& a_states)
{
    const bool drawing = draw(a_spriteSheetFilePath, a_textureCoords.m_leftCoord,
                              a_textureCoords.m_topCoord, a_x,a_y,a_states);

    // Error Handling
    if (drawing == false)
    {
        std::cerr << "Failed to draw Graphics \n";
    }
}

//Draw From Animation Class:
void Graphics::draw(const Animation& a_animation,
    const unsigned short& a_x, const unsigned short& a_y, const sf::RenderStates& a_states)
{
    m_sprite.setScale(a_animation.m_xScale, a_animation.m_yScale);

    // Pulls File Path From Animation Data,
    // Pulls FrameCoords From Animation Data using m_currentFrame From Animation class,
    // Uses Given a_x and a_y, a_states arguments
    const bool drawing = draw(a_animation.m_animationData.m_spriteSheetFilePath,
        a_animation.m_animationData.m_frameCoords[a_animation.m_currentFrame].m_leftCoord,
        a_animation.m_animationData.m_frameCoords[a_animation.m_currentFrame].m_topCoord,
        a_x,a_y,a_states);
    
    m_sprite.setScale(1,1);
    
    if (drawing == false)
    {
        std::cerr << "Failed to draw Graphics \n";
    }
}

#pragma endregion GraphicsClass