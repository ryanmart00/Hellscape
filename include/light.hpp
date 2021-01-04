#ifndef LIGHT_HPP_INCLUDED
#define LIGHT_HPP_INCLUDED

#include <glm/glm.hpp>
#include "shader.hpp"
class BaseLight
{
public:
    BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular); 

    /**
     *  Assigns this light to shader.
     */
    virtual void assignToShader(Shader& shader, unsigned int num) = 0;
    
protected:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_; 
};

class DirectionalLight : public BaseLight
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, 
        glm::vec3 specular);

    /**
     *  Assigns this light to shader.
     *  Note that the shader must have an array of structs named directionalLights
     *  with vec3 properties 
     *      - direction
     *      - ambient
     *      - diffuse
     *      - specular
     */
    virtual void assignToShader(Shader& shader, unsigned int num);

protected:
    glm::vec3 direction_;

};

class PointLight : public BaseLight
{
public:
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, 
        glm::vec3 specular, float constFalloff, float linearFalloff, float quadFalloff);

    /**
     *  Assigns this light to shader.
     *  Note that the shader must have an array of structs named pointLights
     *  with vec3 properties 
     *      - position
     *      - ambient
     *      - diffuse
     *      - specular
     *  and float properties
     *      - constant
     *      - linear
     *      - quadratic
     */
    virtual void assignToShader(Shader& shader, unsigned int num);

    glm::vec3 position_;

protected:
    float constFalloff_;
    float linearFalloff_;
    float quadFalloff_;

};

class DirectionalShadowLight : public DirectionalLight
{
private:
    unsigned int depthMapFBO;
    unsigned int depthMap;
};

class PointShadowLight : public PointLight
{

};

#endif
