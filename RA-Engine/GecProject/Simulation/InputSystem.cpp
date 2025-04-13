// Includes:
#include "InputSystem.h"
#include <assert.h>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

// Input Interface:
// Default Constructor - Adds Interface to Input System
InputInterface::InputInterface()
{
    INPUT_SYSTEM.addInterface(this);
}

// Default De-Constructor - Removes Interface From Input System
InputInterface::~InputInterface()
{
    INPUT_SYSTEM.removeInterfaces(this);
}

// Static:
InputSystem* InputSystem::m_instance = nullptr;
std::list<InputInterface*> InputSystem::m_inputInterfaces = {};

// Singleton:
void InputSystem::createInstance(void)
{
    if (m_instance == nullptr)
    {
        m_instance = new InputSystem();
    }
}

InputSystem& InputSystem::getInstance(void)
{
    // Return m_instance reference
    #if defined(_DEBUG) 
        assert(m_instance != nullptr);
    #endif

    return *m_instance;
}

// Removing Instance of Singleton
void InputSystem::destroyInstance(void)
{
    #if defined(_DEBUG) 
        assert(m_instance != nullptr);
    #endif

    delete m_instance;
    m_instance = nullptr;
}

// Member Functions:
// Update - Logs inputs before passing them to input interface children 
void InputSystem::update()
{
    // Brute Force Logging Inputs:
    m_loggedInputs.clear();

    // Directions:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_loggedInputs.push_back(Left);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_loggedInputs.push_back(Right);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_loggedInputs.push_back(Up);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_loggedInputs.push_back(Down);
    }

    // Operators:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    {
        m_loggedInputs.push_back(Start);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        m_loggedInputs.push_back(Select);
    }

    // Buttons:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        m_loggedInputs.push_back(Button1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
    {
        m_loggedInputs.push_back(Button2);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        m_loggedInputs.push_back(Button3);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
    {
        m_loggedInputs.push_back(Button4);
    }

    // Updating Interface Children:
    for (InputInterface*& currentInterface : m_inputInterfaces)
    {
        currentInterface->updateInput(m_loggedInputs);
    }
}


// Interface Control:
// Returns true or false depending on if interface is in list:
bool InputSystem::checkInterfaceIsInList(const InputInterface* a_inputInterface)
{
    if (std::find(m_inputInterfaces.begin(),m_inputInterfaces.end(),a_inputInterface) != m_inputInterfaces.end())
    {
        return true;
    }
    return false;
}

// Adds to the list if input isn't already in the list:
void InputSystem::addInterface(InputInterface* a_inputInterface)
{
    // Checking if interface isn't in list
    if (checkInterfaceIsInList(a_inputInterface) == false)
    {
        m_inputInterfaces.push_back(a_inputInterface);
    }
    else // is in list:
    {
        #if defined(_DEBUG)
                std::cerr << "Attempting to add an interface that already exists within the interface list in the input system : addInterface : InputSystem.cpp" << "\n"; 
        #endif
    }
}

// removes interface from list if interface exists within list
void InputSystem::removeInterfaces(InputInterface* a_inputInterface)
{
    // Checking if interface is in list:
    if (checkInterfaceIsInList(a_inputInterface) == true)
    {
        m_inputInterfaces.remove(a_inputInterface);
    }
    else // not in list:
    {
        #if defined(_DEBUG)
                std::cerr << "Attempting to remove an interface that already exists within the interface list in the input system : addInterface : InputSystem.cpp" << "\n"; 
        #endif
    }
}

// Sets data to the interfaces pointer vector:
void InputSystem::setInterfaces(const std::list<InputInterface*>& a_InputInterfaceVector)
{
    m_inputInterfaces = a_InputInterfaceVector;
}

// Clears the list of interfaces
void InputSystem::clearInterfaces()
{
    m_inputInterfaces.clear();
}


