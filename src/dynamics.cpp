#include "dynamics.hpp"

Dynamics::Dynamics(btVector3 gravity)
{
    collisionConfig_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfig_);
    broadphase_ = new btDbvtBroadphase();
    solver_ = new btSequentialImpulseConstraintSolver();
    world_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfig_);
    world_->setGravity(gravity);
#ifdef DEBUG
    debug_ = new DebugDrawer{};
    world_->setDebugDrawer(debug_);
    world_->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
#endif
}

Dynamics::~Dynamics()
{
    delete world_;
    delete dispatcher_;
    delete broadphase_;
    delete collisionConfig_;
    delete solver_;
#ifdef DEBUG
    delete debug_;
#endif
}

btRigidBody* Dynamics::addRigidBody(btRigidBody* body)
{
    bodies_.push_back(body);
    world_->addRigidBody(body);
    return body;
}

void Dynamics::removeRigidBody(btRigidBody* body)
{
    world_->removeRigidBody(body);
}

void Dynamics::stepSimulation(float dt)
{
    world_->stepSimulation(dt);
}

#ifdef DEBUG
void Dynamics::debugDraw(glm::mat4 proj, glm::mat4 view)
{
    debug_->SetMatrices(view, proj);
    world_->debugDrawWorld();
    debug_->drawLinesWorld();
}
#endif

