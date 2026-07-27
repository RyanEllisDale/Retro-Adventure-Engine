// Contains Interface class to tell game objects how to handle inputs
// Contains Manager class to process inputs and convert them between formats
// Contains Input enum data 

// Config:
#pragma once

// Includes:
#include <list>
#include <vector>

// Global:
#define INPUT_SYSTEM InputSystem::getInstance()

// Inputs:
// Supports D-PAD / WASD Movement + Face Buttons / Key Mappings
// Modeled after Xbox Controllers, Keyboard, and Common Emulator Configurations
enum Input : std::uint8_t
{
    Left,
    Right,
    Up,
    Down,

    Start,
    Select,

    Button1,
    Button2,
    Button3,
    Button4
};

// Interface:
// Abstract Base Class, to be derived from,
// Interface Identifier that tells child class it can take inputs
class InputInterface
{
private:
public:
    // Constructor:
    InputInterface();
    virtual ~InputInterface();
    
    // Pure Input Function,
    // Must Be Overriden by Children:
    virtual void updateInput(const std::vector<Input>& a_inputs) = 0;
};


// Updates Interface Classes, calls the handleInput function
// Singleton system class 
class InputSystem
{
private:
    // Member Variables:
    static std::list<InputInterface*> m_inputInterfaces;
    static InputSystem* m_instance;

    std::vector<Input> m_loggedInputs;

    // Construction:
    InputSystem() = default;
    ~InputSystem() = default;

public:
    // Singleton:
    static void createInstance(void);
    static InputSystem& getInstance(void);
    static void destroyInstance(void);
    
    // Member Functions:
    void update(void);
    
    // Interfaces:
    static bool checkInterfaceIsInList(const InputInterface* a_inputInterface);
    static void addInterface(InputInterface* a_inputInterface);
    static void removeInterfaces(InputInterface* a_inputInterface);
    static void setInterfaces(const std::list<InputInterface*>& a_InputInterfaceVector);
    static void clearInterfaces(void);
};