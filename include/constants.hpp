#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED
#include "btVector3.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <string>
#include "assimp/types.h"

//*** conversions ***
static glm::vec3 convert(btVector3& vec)
{
    return glm::vec3{vec.x(), vec.y(), vec.z()};
}

static btVector3 convert(glm::vec3 vec)
{
    return btVector3{vec.x, vec.y, vec.z};
}

static glm::mat4 convert(aiMatrix4x4& mat)
{
    //TODO: is this transposed?
    return glm::mat4{mat.a1, mat.a2, mat.a3, mat.a4, mat.b1, mat.b2, mat.b3, mat.b4, mat.c1, mat.c2, mat.c3, mat.c4, mat.d1, mat.d2, mat.d3, mat.d4};
}

const glm::vec3 UP{0.0f, 1.0f, 0.0f};

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
const unsigned int DIR_SHADOW_WIDTH = 1 << 13;
const unsigned int DIR_SHADOW_HEIGHT = 1 << 13;

const unsigned int POINT_SHADOW_HEIGHT = 1 << 10;
const unsigned int POINT_SHADOW_WIDTH = 1 << 10;


//***Directional Light Shadow constants***

// after this distance from the center, shadows are no longer drawn
const float RIGHT_SHADOW_CLIPPING_PLANE = 40;
const float UP_SHADOW_CLIPPING_PLANE = 40;

const float NEAR_SHADOW_CLIPPING_PLANE = 1;
// This is the length from a directional light's position where shadows will be drawn.
// the larger this is the more grainy the shadows.
const float FAR_SHADOW_CLIPPING_PLANE = 100;

// This is the length from a point light's position where shadows will be drawn.
// the larger this is the more grainy the shadows.
const float FAR_POINT_SHADOW_CLIPPING_PLANE = 25;

// This is the distance from the center of view (determined by player postion)
// that shadows will start being rendered at. Setting this to half the 
// far clipping plane minus near clipping plane means the center of shadow will be 
// at the player.
const float DIR_LIGHT_BACKWARD_OFFSET = (FAR_SHADOW_CLIPPING_PLANE-NEAR_SHADOW_CLIPPING_PLANE) 
    / 2.0f;


//*** Bone caps ***
const unsigned int MAX_BONES_PER_VERTEX = 4;
const unsigned int MAX_BONES = 10;


#endif
