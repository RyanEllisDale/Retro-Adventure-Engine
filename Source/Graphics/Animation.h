// an animation class that does the animation work for entities,
// plays animations by indexes m_textureCoordinates and,
// calling the drawEntitiy function in the graphics system

#pragma once

// Include
#include <string>
#include <vector>
#include <../Simulation/Physics/MyVector.h> // texture coords
#include <SFML/System/Clock.hpp> // for animation clock

class Animation
{
private:
    // Animation Data:
    static constexpr float m_frameRate = 0.4f;

    // controls data:
    uint8_t m_currentFrame{0}; 
    uint8_t m_frameCount{0}; 
    bool m_animationLoop{true}; 
    bool m_animationUpdating{true};
    sf::Clock m_animationClock;

    // visual data
    std::string m_tilesheetTextureFilepath{"Data/Textures/MissingTexture.png"};
    std::vector<MyVector> m_textureCoordinates{{0.0f,0.0f}};
    
public:
    // Construction:
    Animation() = default;
    Animation(const Animation&);
    Animation(const std::string a_tilesheetFilePath, const std::vector<MyVector>& a_listOfTextureCoordinates,
        const int a_frameCount, const bool& a_loop = false);
    ~Animation() = default;
    
    // Public Member Function:
    const MyVector* update(void);
    void reset(void);
    void stop(void);
    void play(void);
    void draw(const MyVector& a_position, const uint8_t& a_palleteIndex, const bool& a_xFlip, const bool& a_yFlip) const;
};

