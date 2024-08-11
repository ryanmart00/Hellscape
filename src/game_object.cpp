#include "game_object.hpp"
#include <iostream>

BaseObject::BaseObject(BaseObject* parent, AssetManager& manager) 
    : parent_{parent}, children_{}, manager_{manager}
{
    if(parent_)
    {
        parent_->addChild(this);
    }
}

BaseObject::~BaseObject() 
{
    #ifdef DEBUG
        std::cerr << "BaseObject Destructor" << std::endl;
    #endif
    for(auto i = children_.begin(); i != children_.end(); )
    {
        delete *i;
        i = children_.erase(i);
    }
}

void BaseObject::Draw(Shader* shader, int numShadowMaps)
{
    draw(shader, numShadowMaps);
    for(auto i = children_.begin(); i != children_.end(); ++i)
    {
        (*i)->Draw(shader, numShadowMaps);
    }
}

void BaseObject::Update(Dynamics* world, float dt)
{
    update(world, dt);
    for(auto i = children_.begin(); i != children_.end(); ++i)
    {
        (*i)->Update(world, dt);
    }
}

glm::mat4 BaseObject::convertWorldTransform()
{
    float mat[16];
    btTransform trans;
    getWorldTransform(trans);
    trans.getOpenGLMatrix(mat);
    return glm::make_mat4(mat);
}

void BaseObject::addChild(BaseObject* child)
{
    children_.push_back(child);
}

StaticObject::StaticObject(BaseObject* parent, AssetManager& manager, 
    std::string modelPath, std::string shapePath, btTransform transform) 
    : BaseObject{parent, manager}, model_{nullptr}, rigidBody_{nullptr}, 
    motion_{new btDefaultMotionState{transform}}, modelPath_{modelPath},shapePath_{shapePath},
    tivarr_{nullptr}, shape_{nullptr}, numMeshes_{0}
{
}

StaticObject::~StaticObject()
{
    manager_.unload(modelPath_);
    if(rigidBody_)
    {
        delete rigidBody_;
    }
    if (motion_)
    {
        delete motion_;
    }
    if (shape_)
    {
        delete shape_;
    }
    if (tivarr_)
    {
        delete[] tivarr_;
    }
    if (trimesharr_)
    {
        for(int i = 0; i < numMeshes_; i++)
        {
            delete trimesharr_[i];
        }
        delete[] trimesharr_;
    }
}

void StaticObject::draw(Shader* shader, int numShadowMaps)
{
    shader->setMat4("model", convertWorldTransform());
    model_->Draw(shader, numShadowMaps);
} 

void StaticObject::update(Dynamics*, float)
{
}

void StaticObject::getWorldTransform(btTransform& trans)
{
    motion_->getWorldTransform(trans);
}

void StaticObject::softInit()
{
    manager_.load(modelPath_); 
    manager_.load(shapePath_); 
}

void StaticObject::hardInit(Dynamics* world)
{
    this->BaseObject::hardInit(world);
    model_ = manager_.get(modelPath_);

    Model* shapeModel = manager_.get(shapePath_);
    shape_ = new btCompoundShape{};
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3{0,0,0});
    numMeshes_ = shapeModel->meshes_.size();
    tivarr_ = new btTriangleIndexVertexArray[numMeshes_];
    trimesharr_ = new btTriangleMeshShape*[numMeshes_];
    int index = 0;
    for(auto i = shapeModel->meshes_.begin(); i != shapeModel->meshes_.end(); ++i)
    {
        int numTri = i->indices_.size() / 3;
        int numVert = i->vertices_.size();
        tivarr_[index] = btTriangleIndexVertexArray{
            numTri, (int*) &i->indices_[0], 3*sizeof(unsigned int),
            numVert, (btScalar*) &i->vertices_[0].Position, sizeof(Vertex)};
        trimesharr_[index] = new btBvhTriangleMeshShape{&tivarr_[index], true};
        shape_->addChildShape(trans, trimesharr_[index]);
        index++;
    }
    btVector3 inertia{0,0,0};
    btRigidBody::btRigidBodyConstructionInfo info{0, motion_, shape_, inertia};
    rigidBody_ = new btRigidBody(info);

    world->addRigidBody(rigidBody_);
    //TODO: Can I unload the shape here?
    //manager_.unload(shapePath_);
}

void StaticObject::softDestruct(Dynamics* world)
{
    this->BaseObject::softDestruct(world);
    world->removeRigidBody(rigidBody_);
}

DynamicObject::DynamicObject(BaseObject* parent, AssetManager& manager, 
    btCollisionShape* shape, float mass, std::string modelPath, 
    btTransform transform) 
    : BaseObject{parent, manager}, model_{nullptr}, rigidBody_{nullptr},
    mass_{mass}, shape_{shape}, 
    motion_{new btDefaultMotionState{transform}}, modelPath_{modelPath}
{
}

DynamicObject::~DynamicObject()
{
    manager_.unload(modelPath_);
    if(rigidBody_)
    {
        delete rigidBody_;
    }
    if (motion_)
    {
        delete motion_;
    }
}

void DynamicObject::draw(Shader* shader, int numShadowMaps)
{
    shader->setMat4("model", convertWorldTransform());
    model_->Draw(shader, numShadowMaps);
} 

void DynamicObject::update(Dynamics*, float)
{
}

void DynamicObject::getWorldTransform(btTransform& trans)
{
    motion_->getWorldTransform(trans);
}

void DynamicObject::softInit()
{
    manager_.load(modelPath_); 
}

void DynamicObject::hardInit(Dynamics* world)
{
    this->BaseObject::hardInit(world);
    model_ = manager_.get(modelPath_);
    
    btVector3 inertia{0,0,0};
    shape_->calculateLocalInertia(mass_, inertia);
    btRigidBody::btRigidBodyConstructionInfo info{mass_, motion_, shape_, inertia};
    rigidBody_ = new btRigidBody(info);

    world->addRigidBody(rigidBody_);
}

void DynamicObject::softDestruct(Dynamics* world)
{
    this->BaseObject::softDestruct(world);
    world->removeRigidBody(rigidBody_);
}

