#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

class Camera
{
public:
    glm::vec3 position_;
    glm::quat orientation_;
    glm::mat4 transpose_;

    Camera() = delete;
    Camera(const glm::vec3& pos, const glm::quat& orient);
    Camera(const glm::vec3& pos, const glm::vec3& direction, const glm::vec3& up);

    /**
     * Returns the view matrix:
     *
     * The view matrix is a non-linear transformation that makes the camera's
     * position the origin by translation and rotates the world by orienation,
     * so that the camera's right, up, and -direction vectors are the new basis
     * for all objects in the new coordinate system.
     */
    glm::mat4 view() const;
    
    /**
     * Sets the orientation by setting the forward and up vectors for the camera
     */
    void setOrientation(const glm::vec3& direction, const glm::vec3& up);

    /**
     * Rotates the camera angle radians about axis
     */
    void rotate(float angle, const glm::vec3& axis);

    /**
     * Rotates the camera angle radians about the axis formed by (x, y, z)
     */
    void rotate(float angle, float x, float y, float z);

    /**
     * Returns a vector pointing in the screen forwards direction
     */
    glm::vec3 getForward() const;

    /**
     * Returns a vector pointing in the screen right direction
     */ 
    glm::vec3 getRight() const;

    /**
     * Returns a vector pointing in the screen up direction
     */ 
    glm::vec3 getUp() const;

};


#endif
