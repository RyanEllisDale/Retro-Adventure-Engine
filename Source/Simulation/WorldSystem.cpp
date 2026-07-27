// Includes:
#include "WorldSystem.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "../pugixml/src/pugixml.hpp"
#include "../Entities/Player.h"
#include "../Entities/Sword.h"
#include "../Entities/OcttoEnemy.h"
#include "../Entities/CopterEnemy.h"

#include "../../Graphics/GraphicsSystem.h"
#include "../../Sound/SoundSystem.h"

// Static:
WorldSystem* WorldSystem::m_instance = nullptr;
Entity* WorldSystem::m_player = nullptr;
const float WorldSystem::m_tick = 0.016f; // around 60fps
sf::Clock WorldSystem::m_simulationClock = sf::Clock();
bool WorldSystem::m_isRunning = true;
bool WorldSystem::m_gameOver = false;
Difficulty WorldSystem::m_difficulty = normal;

// Removes all entities pointers
WorldSystem::~WorldSystem()
{
    // Using reverse iterator
    for (std::list<Entity*>::reverse_iterator rit = m_entities.rbegin(); rit != m_entities.rend(); ++rit)
    {
        delete (*rit);
    }
}

// Singleton:
// Creation, Getting, Destroying
void WorldSystem::createInstance(void)
{
    if (m_instance == nullptr)
    {
        m_instance = new WorldSystem();
        m_simulationClock.restart();
    }
}

WorldSystem& WorldSystem::getInstance(void)
{
    // Return m_instance reference
    #if defined(_DEBUG) 
        assert(m_instance != nullptr);
    #endif

    return *m_instance;
}

void WorldSystem::destroyInstance(void)
{
    delete m_instance;
    m_instance = nullptr;
}

// resets the player pointer, 
// pointer can become inaccurate upon loading a level / building due to m_entities re-sizing,
// ensures the world always has a way to look for player
void WorldSystem::findPlayer()
{
    // Finding M_Player
    for (Entity* currentEntity : m_entities)
    {
        if (typeid(currentEntity) == typeid(Player))
        {
            m_player = currentEntity;
        }
    }

#if defined(_DEBUG)
    assert(m_player != nullptr);
#endif
}

// Sets up the cycling for the game in the update function
void WorldSystem::gameOver(void)
{
    m_gameOver = true;
    m_gameOverClock.restart();
}

// Entity Functions:
// returns true or false if an entitiy is in m_entities:
// useful for saftey checking on whether to delete data in m_entities
bool WorldSystem::checkEntityIsInList(const Entity* a_entity) const
{
    if (std::find(m_entities.begin(),m_entities.end(),a_entity) != m_entities.end())
    {
        return true;
    }
    return false;
}

// checks if the entity isn't already in m_entities,
// then adds the new entity to the queue of entities to be added
void WorldSystem::addEntity(Entity* a_entity)
{
    // Checking if entity isn't in list
    if (checkEntityIsInList(a_entity) == false)
    {
        m_entitiesToAdd.push_back(a_entity);
    }
    else // is in list:
    {
        #if defined(_DEBUG)
                std::cerr << "Attempting to add an entity that already exists within the entities list in the world system : addEntity : WorldSystem.cpp" << "\n"; 
        #endif
    }
}

// checks if the entity is in the list and if so removed
// if so adds the entitiy to the queue of entities to be removes
void WorldSystem::removeEntity(Entity* a_entity)
{
    // Checking if interface is in list:
    if (checkEntityIsInList(a_entity) == true)
    {
        m_entitiesToRemove.push_back(a_entity);
    }
    else // not in list:
    {
        #if defined(_DEBUG)
                std::cerr << "Attempting to remove an entity that doesnt exists within the entities list in the world system : removeEntity : WorldSystem.cpp" << "\n"; 
        #endif
    }
}

// increases the score based on the difficulty
void WorldSystem::incrimentScore(void)
{
    if (m_difficulty == hard)
    {
        m_score = m_score + 1;
        return;
    }

    m_score = m_score + 2;
}

// Adds and removes entities to m_entities
// Called after regular update event 
void WorldSystem::updateWorldEntityList()
{
    // Adding Newly Created Entities:
    for (Entity* currentEntity : m_entitiesToAdd)
    {
        m_entities.push_back(currentEntity);
    }

    // Removing Destroyed Entities:
    for (Entity* currentEntity : m_entitiesToRemove)
    {
        delete currentEntity;
        m_entities.remove(currentEntity);
    }

    m_entitiesToAdd.clear();
    m_entitiesToRemove.clear();
}

// spawns an entity based on a string representation found in the level data
// returns the newly added entity or nullptr if the entity couldn't be added
const Entity* WorldSystem::spawnEntity(const std::string& a_entityStringRepresentation,
    const MyVector& a_entityPosition, const bool& a_persistence)
{
    // Entity Pointer
    Entity* newEntity = nullptr;
    
    if (a_entityStringRepresentation == "player")
    {
        newEntity = new Player;
        m_player = newEntity;
    }
    else if (a_entityStringRepresentation == "sword")
    {
        newEntity = new Sword;
    }
    else if (a_entityStringRepresentation == "octto")
    {
        newEntity = new OcttoEnemy;
    }
    else if (a_entityStringRepresentation == "copter")
    {
        newEntity = new CopterEnemy;
    }
    // Error Message:
    else
    {
        std::cerr << "Attempting to spawn an entity without a representation in Spawn Entity:"<< "\n"
                << "Check if entityStringRepresentation is correct and that you have added the representation to the spawn entity function," << "\n"
                << "Additionally, make sure to check your spelling as you may have accidentally mistyped or misplaced a capital." << "\n"
                << "WorldSystem : Spawn Entity" << "\n"; 

        return nullptr;
    }
    
    newEntity->setPosition(a_entityPosition);
    return newEntity;
}

// Loads level from file
// big function that goes through all the critera the level files need
// loads all the data and sets all the member variables / game data based on level filepath
const bool WorldSystem::loadLevel(const std::string& a_levelFilePath)
{
    // Mute File path
    if (a_levelFilePath == "")
    {
        return false;
    }
    
    // Asserting level file exists:
    pugi::xml_document doccument;
    pugi::xml_parse_result result = doccument.load_file(a_levelFilePath.c_str());
    if (result.status != pugi::status_ok )
    {
        std::cerr << "Error loading XML file : " << a_levelFilePath << "\n";
        return false;
    }

    m_levelFilePath = a_levelFilePath;
    
    // Level Data:
    // makes sure file format is correct
    const pugi::xml_node root = doccument.child("root");
#if defined(_DEBUG)
    assert(root);
    assert(root.child("name"));
    assert(root.child("backgroundColour"));
    assert(root.child("tilesheetPath"));
    assert(root.child("collisionIndexes"));
    assert(root.child("tiles"));
    assert(root.child("entities"));
    assert(root.child("surroundingLevels"));
    assert(root.child("pallete"));
    assert(root.child("backgroundMusic"));
#endif

    // Name:
    m_levelName = "";
    m_levelName = root.child("name").child_value();

    // Colour:
    const pugi::xml_node backgroundColourNode = root.child("backgroundColour");
#if defined(_DEBUG)
    assert(backgroundColourNode.child("red"));
    assert(backgroundColourNode.child("green"));
    assert(backgroundColourNode.child("blue"));
#endif

    sf::Color backgroundColor = sf::Color::White;
    const int red = std::stoi(backgroundColourNode.child("red").child_value());
    const int green = std::stoi(backgroundColourNode.child("green").child_value());
    const int blue = std::stoi(backgroundColourNode.child("blue").child_value());
    backgroundColor.r = red;
    backgroundColor.g = green;
    backgroundColor.b = blue;

    // TilesheetPath:
    std::string textureFilePath = root.child("tilesheetPath").child_value();
    
    // Collisions:
    std::vector<int> collisionIndexes;
    for (const pugi::xml_node currentNode : root.child("collisionIndexes"))
    {
        collisionIndexes.push_back(std::stoi(currentNode.child_value()));    
    }

    // Music:
    const std::string backgroundMusicPath = root.child("backgroundMusic").child_value();
    
    if (backgroundMusicPath != m_levelBackgroundMusic)
    {
        SOUND_SYSTEM.playMusic(backgroundMusicPath,0,true);
        m_levelBackgroundMusic = backgroundMusicPath;
    }

    
    // Set Tiles:
    m_map.clear();
    for (const pugi::xml_node currentTileRowNode : root.child("tiles")) // Rows
    {
        // Tiles:
        for (const pugi::xml_node currentTileNode : currentTileRowNode.children())
        {
            const int index = std::stoi(currentTileNode.attribute("index").value());
            
            Tile newTile(index);

            // Tile Settings:
            // Collisions:
            if (std::find(std::begin(collisionIndexes),std::end(collisionIndexes),index) != collisionIndexes.end())
            {
                newTile.m_tileCollidable = true;
            }

            // Flips:
            std::string trueString = "true";
            if (currentTileNode.attribute("xFlip"))
            {
                if (currentTileNode.attribute("xFlip").value() == trueString)
                {
                    newTile.m_settings.m_xFlip = true;
                }
            }
            
            if (currentTileNode.attribute("yFlip"))
            {
                if (currentTileNode.attribute("yFlip").value() == trueString)
                {
                    newTile.m_settings.m_yFlip = true;
                }
            }
            
            // Visibility:
            if (currentTileNode.attribute("visible"))
            {
                if (currentTileNode.attribute("visible").value() == "false")
                {
                    newTile.m_settings.m_visible = false;
                }
            }
            
            m_map.push_back(newTile);
        }
    }

    // Pallete:
    const uint8_t palleteIndex = std::stoi(root.child("pallete").child_value());
    
    // Setting Background:
    GRAPHICS_SYSTEM.setBackground(textureFilePath,m_map, backgroundColor, palleteIndex);

    // Clearing Entities:
    updateWorldEntityList();
    for (Entity* currentEntity : m_entities)
    {
        if (currentEntity->getIsPersistent() == false)
        {
            m_entitiesToRemove.push_back(currentEntity);
        }
    }
    updateWorldEntityList(); // clears m_entitiesToAdd and m_entitiesToRemove, moves all relevant entities to m_entities
    
    // Adding Entities:
    for (const pugi::xml_node currentEntity : root.child("entities"))
    {
        // Persistance:
        bool persistent = false;
        if (currentEntity.attribute("persistant"))
        {
            std::string persistentChar = "true";
            if (currentEntity.attribute("persistant").value() == persistentChar)
            {
                persistent = true;
            }
        }
        
        // Position
        float xPosition = static_cast<float>(std::stoi(currentEntity.attribute("xPosition").value()));
        float yPosition = static_cast<float>(std::stoi(currentEntity.attribute("yPosition").value()));
        MyVector entityPosition(xPosition,yPosition);

        // Spawning Entity:
        std::string entityStringRepresentation = currentEntity.child_value();
        spawnEntity(entityStringRepresentation,entityPosition, persistent);
    }
    
    updateWorldEntityList();
    findPlayer();
    
    // Triggers:
    m_triggers.clear();
    const pugi::xml_node surroundingLevels = root.child("surroundingLevels");
    const pugi::xml_node triggers = surroundingLevels.child("triggers");
    for (const pugi::xml_node currentTrigger : triggers.children())
    {
        // Position:
        float xPosition = static_cast<float>(std::stoi(currentTrigger.attribute("xPosition").value()));
        float yPosition = static_cast<float>(std::stoi(currentTrigger.attribute("yPosition").value()));

        float playerXPosition = static_cast<float>(std::stoi(currentTrigger.attribute("playerXPosition").value()));
        float playerYPosition = static_cast<float>(std::stoi(currentTrigger.attribute("playerYPosition").value()));
        
        LevelTrigger newTrigger(currentTrigger.child_value(), MyVector(xPosition,yPosition), MyVector(playerXPosition,playerYPosition));
        m_triggers.push_back(newTrigger);
    }

    // Bounds:
    const pugi::xml_node bounds = surroundingLevels.child("bounds");
    int iteration = 0;
    for (const pugi::xml_node currentBound : bounds.children())
    {
        if (currentBound.attribute("active").value() == "false")
        {
            m_boundsLevelsFilePaths[iteration] = "";
        }
        else
        {
            m_boundsLevelsFilePaths[iteration] = currentBound.child_value();
        }

        iteration = iteration + 1;
    }
    
    return true;
}

// builds / re-builds the world system as fresh (keeping the difficulty),
// deletes and scraps all the entities and makes a new player
void WorldSystem::build()
{

    // clearing entitiies:
    for (Entity* currentEntity : m_entities) // deletes player too
    {
        delete currentEntity;
    }
    for (Entity* currentEntity : m_entitiesToAdd)
    {
        delete currentEntity;
    }
    for (Entity* currentEntity : m_entitiesToRemove)
    {
        delete currentEntity;
    }
    
    m_entities.clear();
    m_entitiesToAdd.clear();
    m_entitiesToRemove.clear();

    // making player:
    m_player = new Player();
    constexpr int x = 300;
    constexpr int y = 300;
    
    m_player->setPosition({x,y});

    // reset score:
    m_score = 0;
}

// triggers game cycle if the game is over or simulates the world if the game is running
void WorldSystem::update()
{
    if (m_gameOver == true)
    {
        // game over pause:
        if (m_gameOverClock.getElapsedTime().asSeconds() < 2.0f)
        {
            return;
        }

        // restart game:
        const std::string startingLevel = "Data/Levels/Level1.xml";
        build();
        loadLevel(startingLevel);
        m_gameOver = false;
    }

    // game not over:

    // Delta Time:
    m_deltaTime = static_cast<double>(m_deltaClock.getElapsedTime().asMilliseconds()) - m_lastTick;
    m_lastTick = static_cast<double>(m_deltaClock.getElapsedTime().asMilliseconds());

    // Updates:
    // based off a tick
    if (m_simulationClock.getElapsedTime().asSeconds() > m_tick)
    {
        // Simulation Clock
        float simulationTimeScalar = m_simulationClock.getElapsedTime().asSeconds() / m_tick;
        m_simulationClock.restart();

        // Updates:
        INPUT_SYSTEM.update();
        if (GRAPHICS_SYSTEM.windowUpdate() == false)
        {
            m_isRunning = false;
            updateWorldEntityList();
            return;
        }

        GRAPHICS_SYSTEM.drawWorld();
        for (Entity*& currentEntity : m_entities)
        {
            if (currentEntity != nullptr)
            {
                currentEntity->update(m_deltaTime, simulationTimeScalar);
            }
        }
        
        //GRAPHICS_SYSTEM.drawText("Score: " + std::to_string(m_score), MyVector(40, 40));
        GRAPHICS_SYSTEM.display();

        // Triggers:
        // Checking if player has triggered a level change:
        constexpr int spriteSize = SPRITE_SIZE;
        for (const LevelTrigger& trigger : m_triggers)
        {
            if (m_player->getCollisionMask().intersection(trigger.m_position.m_x, trigger.m_position.m_y, trigger.m_position.m_x + spriteSize / 2, trigger.m_position.m_y + spriteSize / 2) == true)
            {
                m_player->setPosition(trigger.m_playerPosition);
                loadLevel(trigger.m_levelFilePath);
            }
        }

        // Bounds:
        constexpr int worldWidth = WORLD_WIDTH;
        constexpr int worldHeight = WORLD_HEIGHT;

        const MyVector playerPosition = m_player->getPosition();
        if (playerPosition.m_x + spriteSize < 0) // left
        {
            m_player->setPosition(MyVector(static_cast<float>(worldWidth - spriteSize), playerPosition.m_y));
            loadLevel(m_boundsLevelsFilePaths[0]);
        }
        else if (playerPosition.m_y + spriteSize < 0) // up
        {
            m_player->setPosition(MyVector(playerPosition.m_x, static_cast<float>(worldHeight - spriteSize)));
            loadLevel(m_boundsLevelsFilePaths[1]);
        }
        else if (playerPosition.m_x > worldWidth) // right
        {
            m_player->setPosition(MyVector(0.0, playerPosition.m_y));
            loadLevel(m_boundsLevelsFilePaths[2]);
        }
        else if (playerPosition.m_y > worldHeight) // down
        {
            m_player->setPosition(MyVector(playerPosition.m_x, 0.0));
            loadLevel(m_boundsLevelsFilePaths[3]);
        }

        updateWorldEntityList();
    }
}
