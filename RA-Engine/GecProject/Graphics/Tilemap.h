// tilemap class used for the world map
// consists of 15 rows with 16 collumns, each tile is a struct of 3 bytes
// tiles can be visible/invisible, flipped visually, and  have collision, 

// Config:
#pragma once

// Includes:
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <vector>

// Global Scope:
#define TILE_ROWS 15
#define TILE_COLlUMNS 16

// Converting 3 bools into bitset to reduce size
// has a size and alignment of 1 to make the tile struct have a size and alignment of 3
// packed bits
struct TileSettings
{
    /*
    bool m_visible{true};
    bool m_xFlip{false};
    bool m_yFlip{false};
    */
    uint8_t m_visible : 1;
    uint8_t m_xFlip: 1;
    uint8_t m_yFlip: 1;
    
    uint8_t m_unused: 5; // Fills out the last 5 bits in the byte to remove automatic padding

    // Construction,
    TileSettings() = default;
    
    // sets all the vars
    TileSettings(const uint8_t a_visible, const uint8_t a_xFlip, const uint8_t a_yFlip)
        : m_visible(a_visible), m_xFlip(a_xFlip), m_yFlip(a_yFlip), m_unused(0)
    {}
};

// held within the world as map data, used for collisions and graphics
// we have 240 tiles so they need to be minimal:
// Memory optimizations - bitset + uint8_t
struct Tile // 3 bytes 
{
    // Unsigned 8-bit integer (0 - 255):
    // allows for 256 different tiles to be in one tilesetfile
    // 256 different tiles at a size of 32 pixels means 8192 pixels worth in one file
    uint8_t m_tileIndex{0};

    // Packed Settings Booleans:
    TileSettings m_settings{1,0,0};
    
    // On its own for added performance 
    bool m_tileCollidable{false};

    // construction:
    Tile() = default;
    Tile(const uint8_t& a_tileIndex, const bool& a_isCollidable = false,
        const uint8_t a_visible = true, const uint8_t a_xFlip = false, const uint8_t a_yFlip = false)
        : m_tileIndex(a_tileIndex), m_tileCollidable(a_isCollidable)
    {
        m_settings.m_visible = a_visible;
        m_settings.m_xFlip = a_xFlip;
        m_settings.m_yFlip = a_yFlip;

        m_settings.m_unused = 0;
    }
    
};

// child of drawable lets the window draw it directly
// collection of vertex data representing tiles on a texture 
// loads and draws quads over texture to render target (sfml window)
class Tilemap final : public sf::Drawable
{
private:
    // Member Variables:
    sf::VertexArray m_vertices; // Vertex array to store tile vertices (quads)
    sf::Texture m_tileset;      // Tileset texture

    //  Private Functions:
    void draw(sf::RenderTarget& a_target, sf::RenderStates a_states) const override;

public:
    // Construction:
    Tilemap();
    Tilemap(const std::string& a_tileset, const std::vector<Tile>& a_tiles);
    
    // Load the tileset texture and loadLevel the tilemap
    const bool load(const std::string& a_tileset, const std::vector<Tile>& a_tiles);
};