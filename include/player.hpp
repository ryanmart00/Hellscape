#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "game_object.hpp"
#include "camera.hpp"
#include <string>
#include "constants.hpp"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

static glm::vec3 quatForward(glm::quat q) 
{
    return q * glm::vec3(0.0f, 0.0f, -1.0f); 
}

static glm::vec3 quatRight(glm::quat q) 
{
    return q * glm::vec3(1.0f, 0.0f, 0.0f);
}

static glm::vec3 quatUp(glm::quat q) 
{
    return q * glm::vec3(0.0f, 1.0f, 0.0f);
}


class Player : public DynamicObject, public Input
{
public:

    Player() = delete;
    Player(const Player&) = delete;
    Player(AssetManager*, btTransform, glm::vec3, glm::vec3, GameState&); 
    virtual ~Player();

    const glm::mat4 getCameraView();

    const glm::vec3 getCameraPos();

    virtual void hardInit(Dynamics*);
    virtual void mouseCallback(GLFWwindow*, double xpos, double ypos);
    virtual void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
    virtual void pollInput(GLFWwindow*, float dt);
    virtual int getPoints();

protected:
    virtual void update(Dynamics*, float);

private:
    glm::quat cam_;
    bool firstMouse = true;
    double lastX;
    double lastY;
    bool grounded = false;
    int points = 0; // temporary concept to debug shooting
    void incrementPoints();
};

#endif
