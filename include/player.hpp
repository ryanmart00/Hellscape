#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "game_object.hpp"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "glm/ext/quaternion_float.hpp"
#include "inputs.hpp"
#include "constants.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtc/quaternion.hpp"



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

const float TELEPORT_THRESHOLD = 10.0f;

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

    glm::quat cam_;
    bool grounded = false;

    void moveTo(glm::vec3 position, float dt);
    void rotateTo(glm::quat orient);
    void shoot();

protected:
    virtual void update(Dynamics*, float);

};

#endif
