// The essential game object,
// Encompases the score, difficulty, level, entities, and simulation of the game world
// there is no win condition, the game only cycles when the player gets hit and thusly dies

#pragma once

// Include:
#include "InputSystem.h"
#include "../Entities/Base Entities/Entity.h"

// Global Reference:
#define WORLD_SYSTEM WorldSystem::getInstance()

// Forward Declares:
struct Tile;
class Player;

// Difficulty:
// set from player input
enum Difficulty
{
    normal,
    hard
};
// spawned after reading level data,
// Level triggers load levels and set the players position after loading that level
// they are set from the level files directly
struct LevelTrigger
{
    std::string m_levelFilePath;
    MyVector m_position;
    MyVector m_playerPosition;

    LevelTrigger() = default;
    LevelTrigger(const std::string& a_levelFilePath, const MyVector& a_position, const MyVector& a_playerPosition)
        : m_levelFilePath(a_levelFilePath), m_position(a_position), m_playerPosition(a_playerPosition) {}
};

class WorldSystem
{
private:
    friend class Player;

    // Static:
    static WorldSystem* m_instance;
    static Entity* m_player;;
    static sf::Clock m_simulationClock;
    static const float m_tick;
    static Difficulty m_difficulty;
    static bool m_isRunning;
    static bool m_gameOver;

    // Member Variables:
    // Level:
    std::string m_boundsLevelsFilePaths[4]; // left, top, right, bottom
    std::string m_levelBackgroundMusic;
    std::string m_levelFilePath;
    std::string m_levelName;
    std::vector<Tile> m_map;
    std::vector<LevelTrigger> m_triggers;

    // Entities:
    std::list<Entity*> m_entities;
    std::vector<Entity*> m_entitiesToAdd;
    std::vector<Entity*> m_entitiesToRemove;

    sf::Clock m_gameOverClock;

    // Delta Time:
    sf::Clock m_deltaClock;
    double m_lastTick{0.0};
    double m_deltaTime{ 0.0 };
    int m_score{ 0 };

    // Singleton Pointer:
    // Construction:
    WorldSystem(void) = default;
    ~WorldSystem(void);


    // Entities:
    bool checkEntityIsInList(const Entity* a_entity) const;
    void updateWorldEntityList(void);
    const Entity* spawnEntity(const std::string& a_entityStringRepresentation, const MyVector& a_entityPosition,
        const bool& a_persistence);
    void findPlayer(void);

    // Game-cycles:
    void gameOver(void);
    
public:
    // Public Functions::
    // Singleton:
    static void createInstance(void);
    static WorldSystem& getInstance(void);
    static void destroyInstance(void);

    // Running functions:
    const bool& getIsRunning(void) const { return m_isRunning; }
    void update();

    // building / loading the world:
    const bool loadLevel(const std::string& a_levelFilePath);
    const std::vector<Tile>& getMap(void) const { return m_map; }
    void build();

    // Entities:
    const std::list<Entity*>& getEntities(void) const { return m_entities; }
    void addEntity(Entity* a_entity);
    void removeEntity(Entity* a_entity);

    // Game Params:
    void incrimentScore(void);
    static const Difficulty& getDifficulty(void) { return m_difficulty; }
};

