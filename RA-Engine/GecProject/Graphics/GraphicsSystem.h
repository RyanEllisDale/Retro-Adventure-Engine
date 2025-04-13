// graphics class, handles all the visuals for the game,
// consists of pallete data (collection of vec3 colours intended to be used with the shader)
// has its own shader that replaces colours based on index (lets you swap colours in real time, 
// emulating how older systems used to achieve differently coloured sprites, avoids the use of extra texture files)

// Config:
#pragma once

// Global:
#define GRAPHICS_SYSTEM GraphicsSystem::getInstance()
#define SPRITE_SIZE 32
#define WORLD_WIDTH 512
#define WORLD_HEIGHT 480

// Include:
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "../../Simulation/Physics/MyVector.h"
#include "Tilemap.h"

// Collection of vec 3 colour information for the shader
// index 0: replaces all (1.0f,1.0f,1.0f) / pure white colours in texture
// index 1: replaces all (1.0f,0.0f,0.0f) / pure red colours in texture
// index 2: replaces all (0.0f,1.0f,0.0f) / pure green colours in texture
// index 3: replaces all (0.0f,0.0f,1.0f) / pure blue colours in texture
// do not use pure white, red, green or blue colours in your palletes,
// instead consider using 0.98f instead of 1.0f
struct Pallete
{
    sf::Glsl::Vec3 m_colours[4] =
    {
        sf::Glsl::Vec3(0.0f,0.0f,0.0f),
        sf::Glsl::Vec3(0.0f,0.0f,0.0f),
        sf::Glsl::Vec3(0.0f,0.0f,0.0f),
        sf::Glsl::Vec3(0.0f,0.0f,0.0f)
    };
    
    Pallete() = default;
    Pallete(const sf::Glsl::Vec3& index0, const sf::Glsl::Vec3& index1, const sf::Glsl::Vec3& index2, const sf::Glsl::Vec3& index3)
    {
        m_colours[0] = index0;
        m_colours[1] = index1;
        m_colours[2] = index2;
        m_colours[3] = index3;
    }
};

// Main graphics singleton:
// holds the window for the game (closing the window ends the program)
// has all the visual functionality for the world to use
// to create transparency, use the rgb values 128,128,128 on your tileset texture directly
class GraphicsSystem
{
private:
    //static:
    static GraphicsSystem* m_instance;
    static Pallete m_shaderTargetIndexColours;
    static Pallete m_tilesetPalletes[4];
    static Pallete m_entityPalletes[4];

    // Window Data:
    sf::RenderWindow m_window;
    sf::View m_viewport{ sf::Vector2f(static_cast<float>(WORLD_WIDTH) / 2, static_cast<float>(WORLD_HEIGHT) / 2),
        sf::Vector2f(static_cast<float>(WORLD_WIDTH), static_cast<float>(WORLD_HEIGHT))};

    // Text:
    sf::Text m_text;
    sf::Font m_defaultFont;

    // Map / Background / World:
    Tilemap m_tilemap; 
    sf::Color m_backgroundColor{ 255, 255, 255 };  // sfml window clear (clears the whole screen with this colour)
    sf::Color m_alphaMask{ 128, 128, 128 }; // alpha masking the texture before applying shader

    // Frame Management:
    sf::Clock m_frameClock;
    int m_FPS{ 0 };
    int m_currentFrame{ 0 };

    // Draw Data:
    std::unordered_map<std::string, sf::Texture> m_textures;      // Path // Texture
    std::unordered_map<std::string, sf::VertexArray> m_entityQuads; // Path // Bounds

    // Shader:
    sf::Shader m_shader;
    sf::RenderStates m_state;

    // Tilemap Palette and Related Data (4-byte alignment):
    uint8_t m_tileMapPalleteIndex{ 0 };
    std::string m_mapTilesetFilepath;

    GraphicsSystem(void);
    GraphicsSystem(GraphicsSystem&) = delete;

    // Private Functions:
    void clear(void);
    void createQuad(const std::string& a_filename);
    sf::VertexArray& loadQuad(const std::string& a_filename);
    const sf::Texture& loadTexture (const std::string& a_filename);

public:
    // Singleton:
    static void createInstance(void);
    static GraphicsSystem& getInstance(void);
    static void destroyInstance(void);

    // Member Functions:
    bool windowUpdate(void);
    void moveViewport(const MyVector& a_movementVector); // camera
    
    // Drawing:
    void drawWorld();
    void drawEntity(const std::string& a_textureFilePath, const MyVector& a_positionVector,
        const MyVector& a_textureCoords, const uint8_t& a_pallete, 
        const bool& a_isFlippedHorizontally = false, const bool& a_isFlippedVertically = false);
    void drawText(const std::string& a_textToDraw, const MyVector& a_positionVector,
        const sf::Color a_textColor = sf::Color::White, const int a_textSize = 24);
    void display(void); 

    // Setters:
    void setBackground(const std::string& a_tilesetFilepath, const std::vector<Tile>& a_tiles,
        const sf::Color& a_backgroundColour, const uint8_t a_tileMapPalleteIndex);
};
