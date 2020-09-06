#ifndef MY_DYNAMICS_HPP
#define MY_DYNAMICS_HPP

#include <btBulletDynamicsCommon.h>
#include <vector>
#include "debug_drawer.hpp"
#include <glm/glm.hpp>

/**
 * A class that keeps track of all bodies added to the dynamics world (Bullet).
 * Note: bodies added to Dynamics are NOT deleted
 */
class Dynamics
{
public: 
    Dynamics(btVector3 gravity);
    ~Dynamics();

    /**
     * Adds a rigid body to the dynamics world
     */
    btRigidBody* addRigidBody(btRigidBody* body);

    void stepSimulation(float dt);

    void debugDraw(glm::mat4 proj, glm::mat4 view);

    std::vector<btRigidBody*> bodies_;
private:
    btDynamicsWorld* world_;
    btDispatcher* dispatcher_;
    btBroadphaseInterface* broadphase_;
    btConstraintSolver* solver_;
    btCollisionConfiguration* collisionConfig_;
    DebugDrawer* debug_;
};
#endif
