#include "player.hpp"

Player:: Player(AssetManager* manager, btTransform transform, 
    glm::vec3 camDirection, glm::vec3 camUp)
    : DynamicObject{nullptr, manager, new btCapsuleShape{PLAYER_RADIUS,PLAYER_HIEGHT},
    PLAYER_MASS, PLAYER_MODEL_PATH, transform}
{
    cam_ = glm::quatLookAt(camDirection, camUp);
}

Player::~Player()
{
    #ifdef DEBUG
        std::cerr << "Player Destructor" << std::endl;
    #endif
    delete shape_;
}

void Player::mouseCallback(GLFWwindow*, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    double dx = -xpos + lastX;
    double dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    dx *= PLAYER_MOUSE_SENSITIVITY;
    dy *= PLAYER_MOUSE_SENSITIVITY;

    //TODO: stop player from looking all the way around vertically.
    glm::vec3 right = quatRight(cam_);
    cam_ = glm::angleAxis((float)dx, UP) * glm::angleAxis((float)dy, right)* cam_;
    
}

void Player::hardInit(Dynamics* world)
{
    DynamicObject::hardInit(world);
    // We'll be taking care of the gravity
    rigidBody_->setFlags(btRigidBodyFlags::BT_DISABLE_WORLD_GRAVITY);
    rigidBody_->setGravity(btVector3{0,-10,0});
    // This says that the rigid body does not rotate around x or z
    rigidBody_->setAngularFactor(btVector3{0.0f, 1.0f, 0.0f});
    rigidBody_->setActivationState(DISABLE_DEACTIVATION);
}

void Player::pollInput(GLFWwindow *window, float)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
        glm::vec3 v = 10*PLAYER_MASS * glm::cross(UP,quatRight(cam_)) ;
        rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
    }
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
//        cam.position_ -= glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam.getRight()) * dt * 5.0f;
        glm::vec3 v = -10*PLAYER_MASS * glm::cross(UP,quatRight(cam_)) ;
        rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
        glm::vec3 v = -10*PLAYER_MASS * quatRight(cam_);
        rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
//        cam.position_ -= cam.getRight() * dt * 7.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
        glm::vec3 v = 10*PLAYER_MASS * quatRight(cam_);
        rigidBody_->applyCentralForce(btVector3{v.x, v.y, v.z});
//        cam.position_ += cam.getRight() * dt * 7.0f;
	}
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS) 
    {
//        cam.position_ += glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;

        rigidBody_->applyCentralForce(PLAYER_MASS*btVector3{0,1,0});
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
    {
//        cam.position_ -= glm::vec3(0.0f,1.0f,0.0f) * dt * 7.0f;
    }
}

const glm::mat4 Player::getCameraView()
{
    return glm::translate(glm::mat4_cast(glm::conjugate(cam_)), -getCameraPos());
}

const glm::vec3 Player::getCameraPos()
{
    btTransform world;
    getWorldTransform(world);
    return glm::vec3(world.getOrigin().x(), world.getOrigin().y(), world.getOrigin().z()) 
        + PLAYER_CAMERA_UP_OFFSET - (quatForward(cam_) * PLAYER_CAMERA_RADIUS);
}

void Player::update(Dynamics* world, float)
{
    // Spring stuff
    const btVector3& from = rigidBody_->getCenterOfMassPosition();
    const btVector3 to = from + btVector3{0,-2.0f,0};

    // Theoretically this will give us the first non-player hitbox
    ObjectRayCallback callback{rigidBody_};
    callback.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
    world->world_->rayTest(from, to, callback);
    if(callback.hasHit())
    {
        const float pw = 10;
        const float pz = .9f;
        const float poff = 0.3f;

//        std::cerr << callback.m_closestHitFraction << std::endl;
        const btVector3 normal = callback.m_hitNormalWorld;


        rigidBody_->applyCentralForce(PLAYER_MASS*
            ((0.3f - callback.m_closestHitFraction)*pw*pw
            - 2*pw*pz* normal.dot(rigidBody_->getInterpolationLinearVelocity()))*normal);

        const btVector3 playerUp = rigidBody_->getWorldTransform().getBasis() * btVector3{0,1,0};
        rigidBody_->applyTorque(playerUp.cross(normal));

        rigidBody_->setAngularFactor(playerUp);
    }
    else
    {
        rigidBody_->applyCentralForce(-PLAYER_MASS* 10*btVector3{0,1,0});
    }
    rigidBody_->applyCentralForce(-PLAYER_MASS* btVector3
    {
        rigidBody_->getInterpolationLinearVelocity().x(), 
        0, 
        rigidBody_->getInterpolationLinearVelocity().z()
    });
    rigidBody_->applyTorque(-PLAYER_MASS*rigidBody_->getInterpolationAngularVelocity());
    
}

