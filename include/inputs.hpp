#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "text.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <unicode/unistr.h>
#include "GLFW/glfw3.h"
#include "constants.hpp"

    /** InputManager Details
     *  Add to this namespace for each input object, 
     *  - a pointer to the object in objects_
     *  - a pointer to the mouse callback in mouseCallbacks_
     *  - a pointer to the inputPoller in inputPollers_
     *      Input pollers 
     *
     *  Kinda hacky but for now just so that things don't get wonky, InputManager will also 
     *  handle game state: the idea is that there is probably a unique input handler for each 
     *  game state right?
     */
const unsigned int NUM_STATES = 3;
enum GameState
{
    PLAYING = 0,
    SETTINGS = 1,
    MATCH = 2
};


class Input
{
private:
    GameState& state_;
    Input** input_handlers_;
public:

    Input(GameState& state, Input** inputs)
        : state_{state}, input_handlers_{inputs}
    {
    };
    virtual void mouseCallback(GLFWwindow*, double, double) = 0;
    virtual void mouseButtonCallback(GLFWwindow*, int, int, int) = 0;
    virtual void pollInput(GLFWwindow*, float) = 0;
    virtual void characterCallback(GLFWwindow*, unsigned int) = 0;
    virtual void update(float dt) = 0;
    void contextSwitch(GameState to)
    {
        stop();
        input_handlers_[to]->hardInit();
        state_ = to;
    }
    virtual void hardInit() = 0;
    virtual void stop() = 0;

};

class Settings : public Input
{
public:
    Settings(GameState& state, Input** inputs);
    ~Settings() = default;

    virtual void mouseCallback(GLFWwindow*, double xpos, double ypos);
    virtual void mouseButtonCallback(GLFWwindow*, int, int, int);
    virtual void pollInput(GLFWwindow*, float); 
    virtual void characterCallback(GLFWwindow*, unsigned int); 
    virtual void hardInit();
    virtual void stop();
    virtual void update(float dt);
    Text* text_;
};

class Matchmaking : public Input
{
public:
    Matchmaking(GameState& state, Input** inputs); 
    ~Matchmaking() = default;

    virtual void mouseCallback(GLFWwindow*, double xpos, double ypos);
    virtual void mouseButtonCallback(GLFWwindow*, int, int, int);
    virtual void pollInput(GLFWwindow*, float);
    virtual void characterCallback(GLFWwindow*, unsigned int);
    virtual void hardInit();
    virtual void stop();
    virtual void update(float dt);

    std::string ip;
};


#endif
