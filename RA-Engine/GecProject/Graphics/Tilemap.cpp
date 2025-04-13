// Include:
#include "Tilemap.h"

#include <cassert>
#include <cmath>
#include "GraphicsSystem.h"


// Construction
Tilemap::Tilemap()
{
    // Set Up Vertices Array:
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(WORLD_WIDTH / SPRITE_SIZE * (WORLD_HEIGHT / SPRITE_SIZE) * 4);
}

Tilemap::Tilemap(const std::string& a_tileset, const std::vector<Tile>& a_tiles)
{
    // Set Up Vertices Array:
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(WORLD_WIDTH / SPRITE_SIZE * (WORLD_HEIGHT / SPRITE_SIZE) * 4);

    load(a_tileset, a_tiles);
}

// Member Functions:

// overwriting draw function so we can apply texture as a state
void Tilemap::draw(sf::RenderTarget& a_target, sf::RenderStates a_states) const
{
    // Draw the vertex array:
    a_states.texture = &m_tileset;
    a_target.draw(m_vertices, a_states);
}

// Note: Casting makes this function a little awkward:
// returns true if the file loaded properly, false otherwise
// fills in all the tile data based on map data passed from world
const bool Tilemap::load(const std::string& a_tileset, const std::vector<Tile>& a_tiles)
{
    // Load the tileset texture
    if (!m_tileset.loadFromFile(a_tileset))
    {
        return false;
    }

    // Populate the vertex array with one quad per tile
    for (unsigned int i = 0; i < TILE_COLlUMNS; i = i + 1)
    {
        for (unsigned int j = 0; j < TILE_ROWS; j = j + 1)
        {
            // The current tile we are working with:
            // i + j * 16 -> current column (i) + row offset (j) multiplied by total amount of columns
            const Tile& currentTile = a_tiles[i + j * TILE_COLlUMNS];

            // Filter out invisible tiles:
            if (currentTile.m_settings.m_visible == false)
            {
                continue;
            }

            // Casting:
            const int currentTileIndex = currentTile.m_tileIndex;
            const int textureSize = static_cast<int>(m_tileset.getSize().x / 32);
            
            // Find tile coordinates in texture based off tile index,
            // e.g. 3 is the 3rd tile in the texture so its:
                // 3 % (256 / 32) = 3 % (8) = 3 (column 3)
                // 3 / (256 / 32) = 3 / (8) = 0 (row 0)
            const int tileXCoord = currentTileIndex % textureSize; // calculates column
            const int tileYCoord = currentTileIndex / textureSize; // calculates row

            // Note: Casting tileXCoords to floats here makes them inaccurate
            
            // Get a pointer to the current quad's vertices
            sf::Vertex* quad = &m_vertices[static_cast<size_t>(i + j * TILE_COLlUMNS) * 4];

            // Define the quad's four corners,
            // loops through tiles in order so i can automatically position tiles correctly:
                // tiles do not need x and y values
            const float cI = static_cast<float>(i); // casted I - no warnings : ICA
            const float cJ = static_cast<float>(j); // casted J - no warnings : ICA

            // quad corners:
            constexpr int spriteSize = SPRITE_SIZE;
            quad[0].position = sf::Vector2f(cI * spriteSize, (cJ + 1) * spriteSize);
            quad[1].position = sf::Vector2f(cI * spriteSize, cJ * spriteSize);
            quad[2].position = sf::Vector2f((cI + 1) * spriteSize, cJ * spriteSize);
            quad[3].position = sf::Vector2f((cI + 1) * spriteSize, (cJ + 1) * spriteSize);

            // Casting:
            const float cTileXCoord = static_cast<float>(tileXCoord); // casted tileXCoord : ICA
            const float cTileYCoord = static_cast<float>(tileYCoord); // casted tileYCoord : ICA
            
            // Make the texture coordinates for the tiles:
            // set where the bounds should be based on :
                // X = left coord of the texture, Y = top coord of the texture
            sf::Vector2f topLeft = sf::Vector2f(cTileXCoord * spriteSize, (cTileYCoord + 1) * spriteSize);
            sf::Vector2f topRight = sf::Vector2f(cTileXCoord * spriteSize, cTileYCoord * spriteSize);
            sf::Vector2f bottomRight = sf::Vector2f((cTileXCoord + 1) * spriteSize, cTileYCoord * spriteSize);
            sf::Vector2f bottomLeft = sf::Vector2f((cTileXCoord + 1) * spriteSize, (cTileYCoord + 1) * spriteSize);

            // FLipping:
            // Swaps the coords based on the flip:
            
            // Handle horizontal flip (m_xFlip)
            if (currentTile.m_settings.m_xFlip == true)
            {
                std::swap(topLeft, bottomLeft);   // Swap top left and bottom left
                std::swap(topRight, bottomRight); // Swap top right and bottom right
            }

            // Handle vertical flip (m_yFlip)
            if (currentTile.m_settings.m_yFlip == true)
            {
                std::swap(topLeft, topRight);     // Swap top left and top right
                std::swap(bottomLeft, bottomRight); // Swap bottom left and bottom right
            }

            // Finalising the coords:
            quad[0].texCoords = topLeft;
            quad[1].texCoords = topRight;
            quad[2].texCoords = bottomRight;
            quad[3].texCoords = bottomLeft;
        }
    }

    return true;
}
