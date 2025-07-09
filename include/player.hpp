#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "game_object.hpp"
#include "inputs.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "glm/ext/quaternion_float.hpp"

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


class Player : public DynamicObject 
{
public:

    Player() = delete;
    Player(const Player&) = delete;
    Player(AssetManager&, btTransform, glm::vec3, glm::vec3, GameState&); 
    virtual ~Player();

    const glm::mat4 getCameraView();

    const glm::vec3 getCameraPos();

    virtual void hardInit(Dynamics*);
    virtual int getPoints();

    glm::quat cam_;
    bool grounded = false;
protected:
    virtual void update(Dynamics*, float);

private:
    int points = 0; // temporary concept to debug shooting
    void incrementPoints();
};

#endif
