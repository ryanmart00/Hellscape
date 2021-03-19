#include "light.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/integer.hpp"
#include <vector>


BaseLight::BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
    : ambient_{ambient}, diffuse_{diffuse}, specular_{specular}
{
    glGenFramebuffers(1, &depthMapFBO_);

    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D, depthMap_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 (*getCenter)(void), glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular)
    : BaseLight{ambient, diffuse, specular}, direction_{direction}, getCenter_{getCenter}
{
    // Projection is determined by constants in constants.hpp
    projection_ = glm::ortho(-RIGHT_SHADOW_CLIPPING_PLANE, RIGHT_SHADOW_CLIPPING_PLANE,
            -UP_SHADOW_CLIPPING_PLANE, UP_SHADOW_CLIPPING_PLANE, NEAR_SHADOW_CLIPPING_PLANE,
            FAR_SHADOW_CLIPPING_PLANE);
}

void DirectionalLight::assignToShader(Shader& shader, unsigned int num)
{
    shader.use();
    std::string name = "directionalLights[";
    name += std::to_string(num);
    name += "].";
    shader.setVec3(name + "direction", direction_);
    shader.setVec3(name + "ambient", ambient_);
    shader.setVec3(name + "diffuse", diffuse_);
    shader.setVec3(name + "specular", specular_);

}

void DirectionalLight::assignToShadowShader(Shader& shader)
{
    shader.use();
    shader.setMat4("projection", projection_);
}

void DirectionalLight::renderShadows(Shader& shader, std::vector<BaseObject*> renderables)
{
    shader.use();
    shader.setMat4("view", glm::translate(
                glm::mat4_cast(glm::conjugate(glm::quatLookAt(direction_, UP))),
                -getCenter_()+DIR_LIGHT_BACKWARD_OFFSET*direction_));
    glViewport(0,0,SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glClear(GL_DEPTH_BUFFER_BIT);

    // render all objects
    for(auto i = renderables.begin(); i != renderables.end(); i++)
    {
        (*i)->Draw(shader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular, float constFalloff, float linearFalloff, float quadFalloff)
    : BaseLight{ambient, diffuse, specular}, position_{position}, 
    constFalloff_{constFalloff}, linearFalloff_{linearFalloff}, quadFalloff_{quadFalloff}
{

}

void PointLight::assignToShader(Shader& shader, unsigned int num)
{
    shader.use();
    std::string name = "pointLights[";
    name += std::to_string(num);
    name += "].";
    shader.setVec3(name + "position", position_);
    shader.setVec3(name + "ambient", ambient_);
    shader.setVec3(name + "diffuse", diffuse_);
    shader.setVec3(name + "specular", specular_);
    shader.setFloat(name + "constant", constFalloff_);
    shader.setFloat(name + "linear", linearFalloff_);
    shader.setFloat(name + "quadratic", quadFalloff_);
}

void PointLight::assignToShadowShader(Shader&)
{
}

void PointLight::renderShadows(Shader&, std::vector<BaseObject*>)
{
}
