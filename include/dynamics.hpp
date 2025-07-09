#ifndef MY_DYNAMICS_HPP
#define MY_DYNAMICS_HPP

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <glm/glm.hpp>

#ifdef DEBUG
#include "debug_drawer.hpp"
#endif

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

    void removeRigidBody(btRigidBody* body);

    void stepSimulation(float dt);

#ifdef DEBUG
    void debugDraw(glm::mat4 proj, glm::mat4 view);
#endif 

    std::vector<btRigidBody*> bodies_;

    btDynamicsWorld* world_;
private:
    btDispatcher* dispatcher_;
    btBroadphaseInterface* broadphase_;
    btConstraintSolver* solver_;
    btCollisionConfiguration* collisionConfig_;
#ifdef DEBUG
    DebugDrawer* debug_;
#endif
};
#endif
