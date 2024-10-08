#ifndef GAME_OBJECT_HPP_INCLUDED
#define GAME_OBJECT_HPP_INCLUDED

#include "btMotionState.h"
#include "btTransform.h"
#include "btVector3.h"
#include "model.hpp"
#include "shader.hpp"
#include "dynamics.hpp"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "asset_manager.hpp"


/** 
 *  This class is lifted from btKinematicClosestNotMeRayResultCallback from BulletDynamics/Character/btKinematicCharactereController.cpp
 */
class ObjectRayCallback : public btCollisionWorld::ClosestRayResultCallback
{
public:
    ObjectRayCallback (btCollisionObject* me) : btCollisionWorld::ClosestRayResultCallback(btVector3{0.0, 0.0, 0.0}, btVector3{0.0, 0.0, 0.0})
    {
        m_me = me;
    }

    virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == m_me)
        {
            return 1.0;
        }
   
        return ClosestRayResultCallback::addSingleResult (rayResult, normalInWorldSpace);
   }

protected:
    btCollisionObject* m_me;
};

/** The abstract class for Game Objects 
 * 
 * Every subclass must implement: draw, update, getWorldTransform, getVelocity
 *
 * draw and update SHOULD NOT call draw and update for children, that is handled 
 * in Draw and Update (public methods)
 */
class BaseObject
{
public:
    BaseObject() = delete;
    /** Constructs a BaseObject with parent and collision Object*/
    BaseObject(BaseObject*, AssetManager&); 

    virtual ~BaseObject();

    /** Draws the object according to mat4 view and projection and 
     * after draws all children
     */
    void Draw(Shader* shader, int numShadowMaps);

    /** Updates this Object with respect to the delta time and 
     *  after updates all children
     */
    void Update(Dynamics*, float);

    /** Gets the transform of this object */
    virtual void getWorldTransform(btTransform&) = 0;

    /** Begins the initialization process (Load models etc.) */
    virtual void softInit() {};

    /** Ensures that the initialization process is complete 
     * also adds this object to the dynamics world if necessary) */
    virtual void hardInit(Dynamics*) {initialized = true;};

    virtual void softDestruct(Dynamics*) {initialized = false;};

    void addChild(BaseObject*);
protected:
    virtual void draw(Shader* shader, int numShadowMaps) = 0;
    virtual void update(Dynamics*, float) = 0;
    glm::mat4 convertWorldTransform();

    BaseObject* parent_;
    std::vector<BaseObject*> children_;
    AssetManager& manager_;
    bool initialized = false;
    
};

class StaticObject : public BaseObject
{
public:
    StaticObject() = delete;
    StaticObject(const StaticObject&) = delete;
    StaticObject(BaseObject*, AssetManager&, std::string, std::string, btTransform); 
    virtual ~StaticObject();

    
    virtual void getWorldTransform(btTransform&);

    virtual void softInit();
    virtual void hardInit(Dynamics*);

    virtual void softDestruct(Dynamics*);

    Model* model_;
protected:
    virtual void draw(Shader* shader, int numShadowMaps);
    virtual void update(Dynamics*, float);

    btRigidBody* rigidBody_;
    btMotionState* motion_;
    std::string modelPath_;
    std::string shapePath_;
private:
    btTriangleIndexVertexArray* tivarr_; 
    btTriangleMeshShape** trimesharr_;
    btCompoundShape* shape_;
    int numMeshes_;
};

class DynamicObject : public BaseObject
{
public:
    DynamicObject() = delete;
    DynamicObject(const DynamicObject&) = delete;
    DynamicObject(BaseObject*, AssetManager&, btCollisionShape*, float, std::string, 
        btTransform); 
    virtual ~DynamicObject();

    
    virtual void getWorldTransform(btTransform&);

    virtual void softInit();
    virtual void hardInit(Dynamics*);

    virtual void softDestruct(Dynamics*); 

    btRigidBody* rigidBody_;
protected:
    virtual void draw(Shader* shader, int numShadowMaps);
    virtual void update(Dynamics*, float);

    Model* model_;
    float mass_;
    btMotionState* motion_;
    std::string modelPath_;
    btCollisionShape* shape_;
};




#endif

