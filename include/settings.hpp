#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "asset_manager.hpp"

class Settings : public Input
{
public:
    Settings(GameState& state) : Input{state} {};
    ~Settings() = default;

    virtual void mouseCallback(GLFWwindow*, double xpos, double ypos);
    virtual void pollInput(GLFWwindow*, float dt);
};

#endif
