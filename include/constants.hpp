#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED
#include "glm/ext/vector_float3.hpp"
#include <string>

const unsigned int NUM_INPUT_MANAGERS = 1;
const unsigned int PLAYER_INPUT_INDEX = 0;
const glm::vec3 UP{0.0f, 1.0f, 0.0f};

const std::string PLAYER_MODEL_PATH = "assets/Models/Player/player.obj";
const float PLAYER_MASS = 50; // kg
const double PLAYER_MOUSE_SENSITIVITY = 0.005;
const float PLAYER_CAMERA_RADIUS = 3.0f; // m
const glm::vec3 PLAYER_CAMERA_UP_OFFSET = 1.0f * UP; // m vec


#endif
