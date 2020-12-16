#include "game_object.hpp"
#include <iostream>

BaseObject::BaseObject(BaseObject* parent, AssetManager* manager) 
    : parent_{parent}, children_{}, manager_{manager}
{
    if(parent_)
    {
        parent_->addChild(this);
    }
}

BaseObject::~BaseObject() 
{
    std::cerr << "BaseObject Destructor" << std::endl;
    for(auto i = children_.begin(); i != children_.end(); )
    {
        delete *i;
        i = children_.erase(i);
    }
}

void BaseObject::Draw()
{
    draw();
    for(auto i = children_.begin(); i != children_.end(); ++i)
    {
        (*i)->Draw();
    }
}

void BaseObject::Update(float dt)
{
    update(dt);
    for(auto i = children_.begin(); i != children_.end(); ++i)
    {
        (*i)->Update(dt);
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

StaticObject::StaticObject(BaseObject* parent, AssetManager* manager, 
    std::string modelPath, std::string shapePath, Shader& shader, btTransform transform) 
    : BaseObject{parent, manager}, model_{}, shader_{shader}, rigidBody_{}, 
    motion_{new btDefaultMotionState{transform}}, modelPath_{modelPath},shapePath_{shapePath}
{
}

StaticObject::~StaticObject(){}

void StaticObject::draw()
{
    shader_.setMat4("model", convertWorldTransform());
    model_->Draw(shader_);
} 

void StaticObject::update(float)
{
}

void StaticObject::getWorldTransform(btTransform& trans)
{
    motion_->getWorldTransform(trans);
}

void StaticObject::softInit()
{
    #ifdef DEBUG
        std::cout << "Static Soft-Init" << std::endl;
    #endif 
    manager_->load(modelPath_); 
    manager_->load(shapePath_); 
}

void StaticObject::hardInit(Dynamics* world)
{
    model_ = manager_->get(modelPath_);

    Model* shapeModel = manager_->get(shapePath_);
    
    btCompoundShape* shape = new btCompoundShape{};
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3{0,0,0});
    for(auto i = shapeModel->meshes_.begin(); i != shapeModel->meshes_.end(); ++i)
    {
        int numTri = i->indices_.size() / 3;
        int numVert = i->vertices_.size();
        btTriangleIndexVertexArray* array = new btTriangleIndexVertexArray{
            numTri, (int*) &i->indices_[0], 3*sizeof(unsigned int),
            numVert, (btScalar*) &i->vertices_[0].Position, sizeof(Vertex)};
        btTriangleMeshShape* mesh = new btBvhTriangleMeshShape{array, true};
        shape->addChildShape(trans, mesh);
    }
    btVector3 inertia{0,0,0};
    btRigidBody::btRigidBodyConstructionInfo info{0, motion_, shape, inertia};
    rigidBody_ = new btRigidBody(info);

    world->addRigidBody(rigidBody_);
    manager_->unload(shapePath_);
}

DynamicObject::DynamicObject(BaseObject* parent, AssetManager* manager, 
    btCollisionShape* shape, float mass, std::string modelPath, 
    Shader& shader, btTransform transform) 
    : BaseObject{parent, manager}, model_{}, shader_{shader}, rigidBody_{},
    mass_{mass}, shape_{shape}, 
    motion_{new btDefaultMotionState{transform}}, modelPath_{modelPath}
{
}

DynamicObject::~DynamicObject()
{
    delete shape_;
}

void DynamicObject::draw()
{
    shader_.setMat4("model", convertWorldTransform());
    model_->Draw(shader_);
} 

void DynamicObject::update(float)
{
}

void DynamicObject::getWorldTransform(btTransform& trans)
{
    motion_->getWorldTransform(trans);
}

void DynamicObject::softInit()
{
    #ifdef DEBUG
        std::cout << "Dynamic Soft-Init" << std::endl;
    #endif 
    manager_->load(modelPath_); 
}

void DynamicObject::hardInit(Dynamics* world)
{
    model_ = manager_->get(modelPath_);
    
    btVector3 inertia{0,0,0};
    shape_->calculateLocalInertia(mass_, inertia);
    btRigidBody::btRigidBodyConstructionInfo info{mass_, motion_, shape_, inertia};
    rigidBody_ = new btRigidBody(info);

    world->addRigidBody(rigidBody_);
}


