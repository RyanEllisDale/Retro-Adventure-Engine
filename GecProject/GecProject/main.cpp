// Includes:
#include "Externals/ExternalHeaders.h"
#include "Externals/RedirectCout.h"

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Classes/Graphics.h"
#include "Classes/Entities/Player.h"

// Global Scope:
int gameFps;
void defineGui();

int main()
{
    // Start:
    std::cout << "Starting" << "\n";

    // Frame:
    int currentFrameCount = 0;
    sf::Clock fpsClock;
    
    // Turn on memory leak checking
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // Rendering
    sf::RenderWindow window(sf::VideoMode(800, 600), "GEC Milestone 1 Example");
    GRAPHICS.createInstance(window);
    
    // Set up ImGui (the UI library)
    ImGui::SFML::Init(window);    
    sf::Clock uiDeltaClock;

    // Game Entities:
    Player player;
    
    // Game Loop:
    while (window.isOpen())
    {
        // Window Event Polling
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type)  // NOLINT(clang-diagnostic-switch-enum)
            {
                // Closing Event:
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }        

        // Frames:
        currentFrameCount = currentFrameCount + 1;
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0)
        {
            gameFps = currentFrameCount;
            currentFrameCount = 0;
            fpsClock.restart();
        }

        // Updates:
        player.update();
        ImGui::SFML::Update(window, uiDeltaClock.restart());

        
        // Definitions:
        defineGui();
        
        // Drawing:
        window.clear();

        player.draw();
        ImGui::SFML::Render(window); // UI needs drawing last
        
        window.display();
    }

    // End:
    GRAPHICS.destroyInstance();
    ImGui::SFML::Shutdown();
    
    std::cout << "Finished!" << '\n';
    return 0;
}

// GUI:
void defineGui()
{
    // Creating GUI Window:
    ImGui::Begin("GEC");
    
    // GUI Elements:
    ImGui::Text("Game FPS: %d",gameFps); 

    /*
    if (ImGui::Button("Walk Down"))
    {
        gameMilestone1.changeAnimation(0);
    }

    else if (ImGui::Button("Walk Up"))
    {
        gameMilestone1.changeAnimation(1);
    }

    else if (ImGui::Button("Walk Side"))
    {
        gameMilestone1.changeAnimation(2);
    }
    */
    
    // ImGui::Checkbox("Wireframe", &m_wireframe); // A checkbox linked to a member variable
    
    // ImGui::Checkbox("Cull Face", &m_cullFace);

    // ImGui::SliderFloat("Speed", &gAnimationSpeed, 0.01f, 0.3f);	


    
    ImGui::End();
}
