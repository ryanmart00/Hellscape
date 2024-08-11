#include "player.hpp"

Player::Player(AssetManager& manager, btTransform transform, 
    glm::vec3 camDirection, glm::vec3 camUp, GameState& state)
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

void Player::incrementPoints()
{
    points++; // ;)
}

int Player::getPoints()
{
    return points;
}

void Player::hardInit(Dynamics* world)
{
    this->BaseObject::hardInit(world);
    DynamicObject::hardInit(world);
    // We'll be taking care of the gravity
    rigidBody_->setFlags(btRigidBodyFlags::BT_DISABLE_WORLD_GRAVITY);
    rigidBody_->setGravity(btVector3{0,0,0});
    // This says that the rigid body does not rotate around x or z
    rigidBody_->setAngularFactor(btVector3{0.0f, 1.0f, 0.0f});
    rigidBody_->setActivationState(DISABLE_DEACTIVATION);
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
    const float dist = PLAYER_HIEGHT/2.0f + 1;
    const btVector3& from = rigidBody_->getCenterOfMassPosition();
    const btVector3 to = from + btVector3{0,-dist,0};

    // Theoretically this will give us the first non-player hitbox
    ObjectRayCallback callback{rigidBody_};
    callback.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
    world->world_->rayTest(from, to, callback);
    if(callback.hasHit())
    {
        grounded = true;
        const float pw = 20;
        const float pz = .7f;
        const float poff = PLAYER_HIEGHT/2.0f + 0.5f;

        const btVector3 normal = callback.m_hitNormalWorld;


        const btVector3 playerUp = rigidBody_->getWorldTransform().getBasis() * btVector3{0,1,0};


        rigidBody_->applyCentralForce(PLAYER_MASS*
            ((poff - dist*callback.m_closestHitFraction)*pw*pw
            - 2*pw*pz* playerUp.dot(rigidBody_->getInterpolationLinearVelocity()))*playerUp);

        rigidBody_->applyTorque(10*PLAYER_MASS*playerUp.cross(normal));

        //rigidBody_->setAngularFactor(playerUp);
    }
    else 
    {
        grounded = false;
        rigidBody_->applyCentralForce(-PLAYER_MASS* 20*btVector3{0,1,0});
    }

    rigidBody_->applyCentralForce(-10* PLAYER_MASS* btVector3
    {
        rigidBody_->getInterpolationLinearVelocity().x(), 
        0, 
        rigidBody_->getInterpolationLinearVelocity().z()
    });
    rigidBody_->applyTorque(-PLAYER_MASS*rigidBody_->getInterpolationAngularVelocity());
    
}

