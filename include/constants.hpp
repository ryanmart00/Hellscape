#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED
#include "btVector3.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <string>

//*** conversions ***
static glm::vec3 convert(btVector3& vec)
{
    return glm::vec3{vec.x(), vec.y(), vec.z()};
}

static btVector3 convert(glm::vec3 vec)
{
    return btVector3{vec.x, vec.y, vec.z};
}

const glm::vec3 UP{0.0f, 1.0f, 0.0f};

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//TODO: Make sure these always match the constants in ALL shaders.
const int NUM_DIR_LIGHTS = 0;
const int NUM_POINT_LIGHTS = 4;


const unsigned int NUM_INPUT_MANAGERS = 1;
const unsigned int PLAYER_INPUT_INDEX = 0;

const std::string PLAYER_MODEL_PATH = "assets/Models/Player/player.obj";
const float PLAYER_MASS = 50; // kg
const float PLAYER_RADIUS = 0.25f;
// The height of the capsule hitbox is the true height minus twice the radius.
const float PLAYER_HIEGHT = 1.5f - 2*PLAYER_RADIUS;
const double PLAYER_MOUSE_SENSITIVITY = 0.005;
const float PLAYER_CAMERA_RADIUS = 3.0f; // m
const glm::vec3 PLAYER_CAMERA_UP_OFFSET = 1.0f * UP; // m vec

//***General Shadow Constants

// These determine the resolution of shadows.
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;


//***Directional Light Shadow constants***

// after this distance from the center, shadows are no longer drawn
const float RIGHT_SHADOW_CLIPPING_PLANE = 40;
const float UP_SHADOW_CLIPPING_PLANE = 40;

const float NEAR_SHADOW_CLIPPING_PLANE = 1;
// This is the length from a directional light's position where shadows will be drawn.
// the larger this is the more grainy the shadows.
const float FAR_SHADOW_CLIPPING_PLANE = 100;

// This is the distance from the center of view (determined by player postion)
// that shadows will start being rendered at. Setting this to half the 
// far clipping plane minus near clipping plane means the center of shadow will be 
// at the player.
const float DIR_LIGHT_BACKWARD_OFFSET = (FAR_SHADOW_CLIPPING_PLANE-NEAR_SHADOW_CLIPPING_PLANE) 
    / 2.0f;




#endif
