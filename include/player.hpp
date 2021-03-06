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


class Player : public DynamicObject, public InputManager::Input
{
public:

    Player() = delete;
    Player(const Player&) = delete;
    Player(AssetManager*, btTransform, glm::vec3, glm::vec3); 
    virtual ~Player();

    const glm::mat4 getCameraView();

    const glm::vec3 getCameraPos();

    virtual void hardInit(Dynamics*);
    virtual void mouseCallback(GLFWwindow*, double xpos, double ypos);
    virtual void pollInput(GLFWwindow*, float dt);


protected:
    virtual void update(Dynamics*, float);

private:
    glm::quat cam_;
    bool firstMouse = true;
    double lastX;
    double lastY;
};

#endif
