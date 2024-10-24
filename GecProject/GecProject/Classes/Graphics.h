// Config:
#pragma once
// Macros:
#define GRAPHICS Graphics::getInstance()
#define SPRITE_SIZE 64 // All Textures Are 16 x 16
#define ANIMATION_FRAME_RATE 3

// Include:
#include "SFML/Graphics.hpp"
#include <string>
#include <vector>

// Namesapce:
using namespace sf; 

struct TextureCoords
{
    unsigned short m_leftCoord{0};
    unsigned short m_topCoord{0};

    TextureCoords() = default;

    TextureCoords(const unsigned short& a_leftCoord, const unsigned short& a_topCoord)
        : m_leftCoord{a_leftCoord}, m_topCoord{a_topCoord}
    {
    }
};

struct AnimationData
{
    std::string m_name;
    
    // Sprite Sheet:
    std::string m_spriteSheetFilePath;

    // Frame Data:
    std::vector<TextureCoords> m_frameCoords;

    // Construction:
    AnimationData() = default;

    // Passing A Vector of Texture Coords:
    AnimationData(const std::string& a_spriteSheetFilePath, const std::vector<TextureCoords>& a_frameCoords)
        : m_spriteSheetFilePath{a_spriteSheetFilePath}, m_frameCoords{a_frameCoords} {}
};

////////////////////////////////////////////////////////////

class Animation
{
private:
    // Friends:
    friend class Graphics;

    // Animation Data:
    short m_xScale{1};
    short m_yScale{1};
    
    AnimationData m_animationData;
    unsigned short m_currentFrame{0};
    Clock m_frameClock;

public:
    // Construction:
    Animation() = default;
    ~Animation() = default;

    Animation(const std::string& a_spriteSheetFilePath, const std::vector<TextureCoords>& a_frameCoords);

    Animation(struct AnimationData a_animationData);

    // Functions:
    void update();

    void setAnimation(const struct AnimationData& a_animationData);
    void resetAnimation();
    void setScale(int a_xScale = 1, int a_yScale = 1);
};

////////////////////////////////////////////////////////////

class Graphics : public Drawable
{
private:
    // Draw Data:
    std::string m_currentSpriteSheetFilePath;
    Texture m_texture;
    Sprite m_sprite;
    static RenderWindow* m_window; // Saving Render Target
    
    // Singleton:
    Graphics();
    static Graphics* m_instance; // Pointer To Singleton Instance

public:
    
    // Singleton:
    static void createInstance(RenderWindow& a_window);
    static Graphics& getInstance();
    static void destroyInstance();

    // Functions:
    // Draws:
    // Default: // Overridden From sf::Drawable
    void draw(RenderTarget& a_target, const RenderStates a_states = RenderStates::Default) const override;

    // Full Draw Function:
    bool draw(const std::string& a_spriteSheetFilePath,
              const unsigned short& a_leftTextureCoord, const unsigned short& a_opTextureCoord,
              const unsigned short& a_x, const unsigned short& a_y,
              const RenderStates& a_states = RenderStates::Default);

    // Texture Coord Struct Based Draw Function:
    void draw(const std::string& a_spriteSheetFilePath,
              const TextureCoords& a_textureCoords, const unsigned short& a_x, const unsigned short& a_y,
              const RenderStates& a_states = RenderStates::Default);

    // Animation Data Struct Based Draw Function:
    void draw(const AnimationData& a_animationData,
              const unsigned short& a_x, const unsigned short& a_y,
              const RenderStates& a_states = RenderStates::Default);

    // Animation Class Based Draw Function
    void draw(const Animation& a_animation,
              const unsigned short& a_x, const unsigned short& a_y,
              const RenderStates& a_states = RenderStates::Default);
};