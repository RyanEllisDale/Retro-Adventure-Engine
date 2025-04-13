// Includes:
#include "Graphics/GraphicsSystem.h"
#include "Simulation/WorldSystem.h"
#include "Sound/SoundSystem.h"
#include "Simulation/InputSystem.h"

// Standard Library:
#include <random>

// EX:
#include "ExternalHeaders.h"
#include "RedirectCout.h"

int main()
{
    // Config:
    
    // Redirect cout to HAPI
    outbuf ob;
    std::streambuf* sb = std::cout.rdbuf(&ob);

    // Redirect cerr
    outbuferr oberr;
    std::streambuf* sberr = std::cerr.rdbuf(&oberr);

    // Turn on memory leak checking
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Start:

    std::cout << "Starting" << "\n";

    // Random Seed | safer hardware method:
    // tries to generate a random number based on hardware:
    // (No warnings ICA requirement)
    
    std::random_device rng; // makes the hardware rng device for the mt19937 (don't know why it's called that)
    std::mt19937 gen(rng()); // Makes the pseudorandom number gen
    std::uniform_int_distribution<> ranges(0, 999); // Range of values for randomSeed
    int randomSeed = ranges(gen); // Picks between the ranges of the values and sets the random seed
    srand(randomSeed); // Legacy C-Style srand (for rand() use)

    // Old Method:
        // srand(time(nullptr));
        // Random number generator seeded with a disallowed source of seed value will generate a predictable sequence of values [cert-msc51-cpp]
    
    // Mem LEAK
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // Systems:
    SoundSystem::createInstance();
    InputSystem::createInstance();
    WorldSystem::createInstance();
    GraphicsSystem::createInstance();

    WORLD_SYSTEM.build();
    WORLD_SYSTEM.loadLevel("Data/Levels/Level1.xml");

    // Game Loop:
    while (WORLD_SYSTEM.getIsRunning() == true)
    {
        WORLD_SYSTEM.update();
    }

    // End:
    GRAPHICS_SYSTEM.destroyInstance();
    WORLD_SYSTEM.destroyInstance();
    SOUND_SYSTEM.destroyInstance();
    INPUT_SYSTEM.destroyInstance();
    
    std::cout << "Finished!" << '\n';
    
    return 0;
}
