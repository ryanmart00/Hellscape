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
    BaseObject(BaseObject*, AssetManager*); 

    virtual ~BaseObject();

    /** Draws the object according to mat4 view and projection and 
     * after draws all children
     */
    void Draw();

    /** Updates this Object with respect to the delta time and 
     *  after updates all children
     */
    void Update(float);

    /** Gets the transform of this object */
    virtual void getWorldTransform(btTransform&) = 0;

    /** Begins the initialization process (Load models etc.) */
    virtual void softInit() {};

    /** Ensures that the initialization process is complete 
     * also adds this object to the dynamics world if necessary) */
    virtual void hardInit(Dynamics*) {};

    virtual void softDestruct(Dynamics*) {};

    void addChild(BaseObject*);
protected:
    virtual void draw() = 0;
    virtual void update(float) = 0;
    glm::mat4 convertWorldTransform();

    BaseObject* parent_;
    std::vector<BaseObject*> children_;
    AssetManager* manager_;
    
};

class StaticObject : public BaseObject
{
public:
    StaticObject() = delete;
    StaticObject(const StaticObject&) = delete;
    StaticObject(BaseObject*, AssetManager*, std::string, Shader&, btTransform); 
    virtual ~StaticObject();

    virtual void draw();
    
    virtual void update(float);
    
    virtual void getWorldTransform(btTransform&);

    virtual void softInit();
    virtual void hardInit(Dynamics*);

    virtual void softDestruct(Dynamics*) {};

protected:
    Model* model_;
    Shader& shader_;
    btRigidBody* rigidBody_;
    btMotionState* motion_;
private:
    std::string modelPath_;
};




#endif

