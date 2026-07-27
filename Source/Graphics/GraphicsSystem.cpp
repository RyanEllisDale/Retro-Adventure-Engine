// Includes:
#include <cassert>
#include <iostream>
#include "GraphicsSystem.h"
#include "../../Simulation/WorldSystem.h"

// Static Members:
GraphicsSystem* GraphicsSystem::m_instance = nullptr;
Pallete GraphicsSystem::m_shaderTargetIndexColours =
{
    {1.0f,1.0f,1.0f}, // white
    {1.0f,0.0f,0.0f}, // red 
    {0.0f,1.0f,0.0f}, // green
    {0.0f,0.0f,1.0f} // blue
};
Pallete GraphicsSystem::m_tilesetPalletes[4] =
{
    // Pallete 0: GreenSheet
    {
        {0.0f,0.0f,0.0f},
        {0.988f,0.847f,0.659f},
        {0.0f,0.659f,0.0f},
        {0.125f,0.22f,0.925f}
    },

    // Pallete 1: Cave
    {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {0.486f, 0.031f,0.0f},
        {}
    },
{}, // Pallete 2: // unused  
{} // Pallete 3: // Unused
};
Pallete GraphicsSystem::m_entityPalletes[4] =
{
    // Pallete 0 : // Link
    {
        {128.0f,128.0f,128.0f},
        {0.501f,0.815f,0.062f},
        {0.988f,0.596f,0.219f},
        {0.784f,0.298f,0.047f}
    },

    // pallete: 1 // enemy red
    {
        {0.0f,0.0f,0.0f},
        {0.847f,0.156f, 0.0f},
        {0.988f,0.988f,0.988f},
        {0.988f,0.596f,0.219f}
    },

    // Pallete: 2 // enemy Blue (hard mode)
    {
        {0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.658f},
        {0.988f,0.988f,0.988f},
        {0.36f, 0.58f, 0.988f}
    },
    {}
};


// Construction:
GraphicsSystem::GraphicsSystem(void)
{
    // Window:
    m_window.create(sf::VideoMode(WORLD_WIDTH,WORLD_HEIGHT),"Retro Adventure Engine");
    m_window.setView(m_viewport);

    // Draw Data:
    m_defaultFont.loadFromFile("Data/Fonts/Arial.ttf");
    m_text.setFont(m_defaultFont);

    // Shader:
    m_shader.loadFromFile("Graphics/vertexShader.vert", "Graphics/fragmentShader.frag");
    m_shader.setUniformArray("targetColours", m_shaderTargetIndexColours.m_colours, 4);
    m_shader.setUniformArray("replacementColours", m_tilesetPalletes[0].m_colours, 4);
    m_state.shader = &m_shader;
}

// Singleton:
// Creates the instance if one does not exist
// Creates the window
void GraphicsSystem::createInstance()
{
    // Error Handling:
    if (GraphicsSystem::m_instance == nullptr)
    {
        GraphicsSystem::m_instance = new GraphicsSystem();
    }
}

// Sets Pointers To Null
// Deletes Singleton instance
void GraphicsSystem::destroyInstance(void)
{
    delete m_instance;
    m_instance = nullptr;
}

// Return the current single instance of graphics
GraphicsSystem& GraphicsSystem::getInstance(void)
{
    // Return m_instance reference
#if defined(_DEBUG) 
    assert(m_instance != nullptr);
#endif

    return *m_instance;
}


// Window Update:
// returns true if the window is still running and false if the window closes
// clears the screen with the background colour
bool GraphicsSystem::windowUpdate()
{
    // Check window is running:
    if (m_window.isOpen())
    {
        // FPS:
        m_currentFrame = m_currentFrame + 1;
        if (m_frameClock.getElapsedTime().asSeconds() >= 1)
        {
            m_FPS = m_currentFrame;
            m_currentFrame = 0;

            m_frameClock.restart();
        }
        
        // Window Event Polling
        sf::Event event;
        if (m_window.pollEvent(event))
        {
            switch (event.type)  // NOLINT(clang-diagnostic-switch-enum)
            {
                // Closing Event:
                case sf::Event::Closed:
                    m_window.close();
                    return false;
                
            default: break;
            }
        }

        // Background:
        m_window.clear(m_backgroundColor);
        return true;
    }
    
    return false;
}


// simple clear function,
// erases all currently stored data, useful for clearing clutter in bigger projects
void GraphicsSystem::clear()
{
    m_textures.clear();
    m_entityQuads.clear();
}

// loading a texture:
// if the texture doesn't exist, graphics makes one from the filepath
// returns the loaded texture
const sf::Texture& GraphicsSystem::loadTexture(const std::string& a_filename)
{
    // Check If Texture Exists:
    // Texture Does Not Exist:
    if (m_textures.find(a_filename) == m_textures.end())
    {
        // Creating New Texture:
        sf::Image image;
        sf::Texture texture;
        
        // Loading Image From File:
        assert(image.loadFromFile(a_filename));
        if (!image.loadFromFile(a_filename))
        {
            // Loading Missing Image Texture From File:
            if (!image.loadFromFile("Data/Textures/MissingTexture.png"))
            {
                // Failed to load all Image:
                // Manually Create Image:
                image.create(SPRITE_SIZE, SPRITE_SIZE, sf::Color::Blue);
            }
        }

        // Image has loaded:
        // Assign alpha mask to texture:
        image.createMaskFromColor(m_alphaMask);

        assert(texture.loadFromImage(image));
        texture.loadFromImage(image);
        
        // Some Texture Loaded:
        // Adding new texture to map
        m_textures[a_filename] = texture;
    }

    // Texture Exists:
    return m_textures[a_filename];
}

// if no quad exists then we make a quad by loading a texture for that quad,
// then we create a quad and put it in the corrosponding quad array at the same key (string),
// as we did for the texture map
void GraphicsSystem::createQuad(const std::string& a_filename)
{
    // Load Texture:
    const sf::Texture* texture = &loadTexture(a_filename);

    // Make Quad Array:
    const sf::VertexArray quadArray{sf::Quads, 4};

    // Add Quad To Entities:
    m_entityQuads[a_filename] = quadArray;
}

// returns the quad at the specified key from the map,
// if the quad does not exist then a quad is created for that texturefilepath and key
sf::VertexArray& GraphicsSystem::loadQuad(const std::string& a_textureFilename)
{
    // Find if quad already exist for entitiy in map
    if (m_entityQuads.count(a_textureFilename) > 0)
    {
        // Quad Exists:
        // Return Quad:
        return m_entityQuads[a_textureFilename];
    }

    // Make and return new quad:
    createQuad(a_textureFilename);
    return m_entityQuads[a_textureFilename];
}

// Loads a quad for the entity and then sets its position to the entities position,
// then handles flipping the quads depending on the given xFlip and yFlip variables
// then sets the shader with the provided pallete (this lets the entities themselves determine,
// what pallete they wish to use.),
// finally, draw the quads against the corrosponding texture file path with the graphics shader applied.
void GraphicsSystem::drawEntity(const std::string& a_textureFilePath, const MyVector& a_positionVector,
    const MyVector& a_textureCoords, const uint8_t& a_pallete, const bool& a_isFlippedHorizontally, const bool& a_isFlippedVertically)
{
    // Creating Quad Array Pointer:
    sf::Vertex* entityQuad = &loadQuad(a_textureFilePath)[0];

    // Position:
    constexpr int spriteSize = SPRITE_SIZE;
    entityQuad[0].position = sf::Vector2f(a_positionVector.m_x, a_positionVector.m_y + spriteSize);
    entityQuad[1].position = sf::Vector2f(a_positionVector.m_x, a_positionVector.m_y);
    entityQuad[2].position = sf::Vector2f(a_positionVector.m_x + spriteSize, a_positionVector.m_y);
    entityQuad[3].position = sf::Vector2f(a_positionVector.m_x + spriteSize, a_positionVector.m_y + spriteSize);

    // Making Coords:
    sf::Vector2f topLeft = sf::Vector2f(a_textureCoords.m_x, a_textureCoords.m_y);
    sf::Vector2f topRight = sf::Vector2f(a_textureCoords.m_x + spriteSize, a_textureCoords.m_y);
    sf::Vector2f bottomLeft = sf::Vector2f(a_textureCoords.m_x, a_textureCoords.m_y + spriteSize);
    sf::Vector2f bottomRight = sf::Vector2f(a_textureCoords.m_x + spriteSize, a_textureCoords.m_y + spriteSize);
    
    // Handle horizontal flip (m_xFlip)
    if (a_isFlippedHorizontally == true)
    {
        std::swap(topLeft, topRight);     // Swap top left and top right
        std::swap(bottomLeft, bottomRight); // Swap bottom left and bottom right
    }

    // Handle vertical flip (m_yFlip)
    if (a_isFlippedVertically == true)
    {
        std::swap(topLeft, bottomLeft);   // Swap top left and bottom left
        std::swap(topRight, bottomRight); // Swap top right and bottom right
    }
    
    // Texture Coords:
    entityQuad[0].texCoords = bottomLeft;
    entityQuad[1].texCoords = topLeft;
    entityQuad[2].texCoords = topRight;
    entityQuad[3].texCoords = bottomRight;
    
    // Draw:
    m_shader.setUniform("texture", m_textures[a_textureFilePath]);
    m_shader.setUniformArray("replacementColours", m_entityPalletes[a_pallete].m_colours, 4);
    m_state.texture = &m_textures[a_textureFilePath];

    
    m_window.draw(m_entityQuads[a_textureFilePath], m_state); 
}

// Text
// configures m_text before drawing it to the window
void GraphicsSystem::drawText(const std::string& a_textToDraw, const MyVector& a_positionVector,
    const sf::Color a_textColor, const int a_textSize)
{
    // Text Settings
    m_text.setString(a_textToDraw);
    m_text.setPosition(a_positionVector.m_x,a_positionVector.m_y);
    m_text.setFillColor(a_textColor);
    m_text.setCharacterSize(a_textSize);
    
    m_window.draw(m_text);
}

// sets up the shader and draws the tilemap using that shader,
// also draws the fps
void GraphicsSystem::drawWorld()
{
    m_shader.setUniform("texture", m_textures[m_mapTilesetFilepath]);
    m_shader.setUniformArray("replacementColours", m_tilesetPalletes[m_tileMapPalleteIndex].m_colours, 4);
    
    m_window.draw(m_tilemap, &m_shader);
}

void GraphicsSystem::display() 
{
    //drawText("FPS: " + std::to_string(m_FPS), MyVector(400, 40), sf::Color::Red);
    m_window.display();
}

// sets up the background texture, colour and shader for the world map
void GraphicsSystem::setBackground(const std::string& a_tilesetFilepath, const std::vector<Tile>& a_tiles,
    const sf::Color& a_backgroundColour, const uint8_t a_palleteIndex = 0)
{
    // Tile Map:
    m_mapTilesetFilepath = a_tilesetFilepath;
    m_tileMapPalleteIndex = a_palleteIndex;
    m_shader.setUniformArray("replacementColours", m_tilesetPalletes[m_tileMapPalleteIndex].m_colours, 4);

    sf::Texture texture;
    texture.loadFromFile(m_mapTilesetFilepath);
    
    m_textures[m_mapTilesetFilepath] = texture;
    m_tilemap.load(a_tilesetFilepath,a_tiles);

    // Setting background colour:
    m_backgroundColor = a_backgroundColour;
}

// camera movement
void GraphicsSystem::moveViewport(const MyVector& a_movementVector)
{
    m_viewport.move(a_movementVector.m_x,a_movementVector.m_y);
}



