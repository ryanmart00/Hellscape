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
    std::string modelPath, Shader& shader, btTransform transform) 
    : BaseObject{parent, manager}, model_{}, shader_{shader}, rigidBody_{}, 
    motion_{new btDefaultMotionState{transform}}, modelPath_{modelPath}
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
    manager_->load(modelPath_); 
}

void StaticObject::hardInit(Dynamics* world)
{
    model_ = manager_->get(modelPath_);
    
    btCompoundShape* shape = new btCompoundShape{};
    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3{0,0,0});
    for(auto i = model_->meshes_.begin(); i != model_->meshes_.end(); ++i)
    {
        int numTri = i->indices_.size() / 3;
        int numVert = i->vertices_.size();
        btTriangleIndexVertexArray* array = new btTriangleIndexVertexArray{
            numTri, (int*) &i->indices_[0], sizeof(unsigned int),
            numVert, (btScalar*) &i->vertices_[0].Position, sizeof(Vertex)};
        btTriangleMeshShape* mesh = new btBvhTriangleMeshShape{array, true}; 
        shape->addChildShape(trans, mesh);
    }
    btVector3 inertia{0,0,0};
    btRigidBody::btRigidBodyConstructionInfo info{0, motion_, shape, inertia};
    rigidBody_ = new btRigidBody(info);

    world->addRigidBody(rigidBody_);
}


