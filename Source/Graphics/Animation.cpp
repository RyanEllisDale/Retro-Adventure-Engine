//Includes:
#include "Animation.h"
#include "GraphicsSystem.h"

// copy constructor:
Animation::Animation(const Animation& otherAnimation)
{
    // Animation Data:
    m_currentFrame = otherAnimation.m_currentFrame;
    m_frameCount = otherAnimation.m_frameCount;

    m_animationUpdating = otherAnimation.m_animationUpdating;
    m_animationLoop = otherAnimation.m_animationLoop;

    m_animationClock = otherAnimation.m_animationClock;
    m_tilesheetTextureFilepath = otherAnimation.m_tilesheetTextureFilepath;

    m_textureCoordinates = otherAnimation.m_textureCoordinates;
}

// Construction:
// sets all member variables
Animation::Animation(const std::string a_tilesheetFilePath, const std::vector<MyVector>& a_listOfTextureCoordinates,
    const int a_frameCount, const bool& a_loop) : m_tilesheetTextureFilepath(a_tilesheetFilePath), m_frameCount(a_frameCount),
    m_animationLoop(a_loop), m_textureCoordinates(a_listOfTextureCoordinates)
{
}

// Member Functions:

// progress through an animations naturally cycle,
// if the m_texturecoords only have 1 index / value,
// then the animation will be a static image,
// incriments index after m_frameRate in seconds has passed
const MyVector* Animation::update()
{
    // Play Animation:
    if (m_animationUpdating == true)
    {
        // Progressing Animation:
        if (m_animationClock.getElapsedTime().asSeconds() >= m_frameRate)
        {
            m_currentFrame = m_currentFrame + 1;
            if (m_currentFrame >= m_frameCount)
            {
                // Looping Animation:
                if (m_animationLoop == true)
                {
                    m_currentFrame = 0;
                }
                // Pauses on the last frame:
                else
                {

                    m_currentFrame = m_currentFrame - 1;
                    m_animationUpdating = false;
                }
            }
            m_animationClock.restart();
        }
    }
    
    return &m_textureCoordinates[m_currentFrame];
}

// Animation Control:
void Animation::reset()
{
    m_animationClock.restart();
    m_currentFrame = 0;
}

void Animation::stop()
{
    m_animationUpdating = false;
}

void Animation::play()
{
    m_animationUpdating = true;
}

// Drawing general animation
void Animation::draw(const MyVector& a_position, const uint8_t& a_palleteIndex, const bool& a_xFlip = false, const bool& a_yFlip = false) const
{
    GRAPHICS_SYSTEM.drawEntity(m_tilesheetTextureFilepath,a_position,m_textureCoordinates[m_currentFrame],a_palleteIndex,a_xFlip,a_yFlip);
}
