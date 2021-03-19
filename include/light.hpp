#ifndef LIGHT_HPP_INCLUDED
#define LIGHT_HPP_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include "shader.hpp"
#include "constants.hpp"
#include "game_object.hpp"
class BaseLight
{
public:
    BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular); 

    /**
     *  Assigns this light to shader.
     */
    virtual void assignToShader(Shader& shader, unsigned int num) = 0;
    virtual void assignToShadowShader(Shader& shader) = 0;

    virtual void renderShadows(Shader& shader, std::vector<BaseObject*> renderables) = 0;
    unsigned int depthMap_;
    
protected:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_; 

    glm::mat4 projection_;
    unsigned int depthMapFBO_;
};

class DirectionalLight : public BaseLight
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 (*getCenter)(void), glm::vec3 ambient, 
            glm::vec3 diffuse, glm::vec3 specular);

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

    virtual void assignToShadowShader(Shader& shader);

    virtual void renderShadows(Shader& shader, std::vector<BaseObject*> renderables);


protected:
    glm::vec3 direction_;
    glm::vec3 (*getCenter_)(void);

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
    virtual void assignToShadowShader(Shader& shader);

    virtual void renderShadows(Shader& shader, std::vector<BaseObject*> renderables);

    glm::vec3 position_;

protected:
    float constFalloff_;
    float linearFalloff_;
    float quadFalloff_;

};
#endif
