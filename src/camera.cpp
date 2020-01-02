#include "camera.hpp"

Camera::Camera(const glm::vec3 &pos, const glm::quat &orient)
{
    position_ = pos;
    orientation_ = orient;
}

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &direction, const glm::vec3 &up)
{
    position_ = pos;
    orientation_ = glm::quatLookAt(direction, up);
} 


glm::mat4 Camera::view() const
{
    return glm::translate(glm::mat4_cast(glm::conjugate(orientation_)), -position_);
}

void Camera::setOrientation(const glm::vec3& direction, const glm::vec3& up)
{
    orientation_ = glm::quatLookAt(direction, up);
}

void Camera::rotate(float angle, const glm::vec3 &axis)
{
    orientation_ = glm::angleAxis(angle, axis) * orientation_;
}

void Camera::rotate(float angle, float x, float y, float z)
{
    orientation_ = glm::angleAxis(angle, glm::vec3(x, y, z)) * orientation_;
}

glm::vec3 Camera::getForward() const
{
    return orientation_ * glm::vec3(0.0f, 0.0f, -1.0f); 
}

glm::vec3 Camera::getRight() const
{
    return orientation_ * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::getUp() const
{
    return orientation_ * glm::vec3(0.0f, 1.0f, 0.0f);
}
