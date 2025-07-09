#include "light.hpp"

#include <glad/gl.h>
#include "constants.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/quaternion.hpp"


BaseLight::BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
        const char* vs, const char* fs, const char* gs) 
    : shadowShader_{0,0,vs, fs, gs}, ambient_{ambient}, diffuse_{diffuse}, specular_{specular}
{
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular)
    : BaseLight{ambient, diffuse, specular, 
        "assets/gl/dirshadow.vs", "assets/gl/dirshadow.fs", nullptr}, 
    direction_{direction}
{
    glGenFramebuffers(1, &depthMapFBO_);

    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_2D, depthMap_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Projection is determined by constants in constants.hpp
    projection_ = glm::ortho(-RIGHT_SHADOW_CLIPPING_PLANE, RIGHT_SHADOW_CLIPPING_PLANE,
            -UP_SHADOW_CLIPPING_PLANE, UP_SHADOW_CLIPPING_PLANE, NEAR_SHADOW_CLIPPING_PLANE,
            FAR_SHADOW_CLIPPING_PLANE);
    shadowShader_.setMat4("projection", projection_);
}

void DirectionalLight::updateShader(Shader* shader, unsigned int num, glm::vec3 center)
{
    shader->use();
    std::string name = "directionalLights[";
    name += std::to_string(num);
    name += "].";
    shader->setVec3(name + "direction", direction_);
    shader->setVec3(name + "ambient", ambient_);
    shader->setVec3(name + "diffuse", diffuse_);
    shader->setVec3(name + "specular", specular_);

    shader->setMat4("dirLightView[" + std::to_string(num) + "]", 
            glm::translate(
                glm::mat4_cast(glm::conjugate(glm::quatLookAt(direction_, UP))),
                -center+DIR_LIGHT_BACKWARD_OFFSET*direction_));
    shader->setMat4("dirLightProj[" + std::to_string(num) + "]", projection_);

    shader->setInt(name + "shadow", num);
    glActiveTexture(GL_TEXTURE0 + num);
    glBindTexture(GL_TEXTURE_2D, depthMap_);
    shadowShader_.use();
    shadowShader_.setMat4("view", glm::translate(
                glm::mat4_cast(glm::conjugate(glm::quatLookAt(direction_, UP))),
                -center+DIR_LIGHT_BACKWARD_OFFSET*direction_));
    shadowShader_.setMat4("projection", projection_);

}

void DirectionalLight::renderShadows(std::vector<BaseObject*> renderables)
{
    shadowShader_.use();

    glViewport(0,0,DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    // render all objects
    for(auto i = renderables.begin(); i != renderables.end(); i++)
    {
        (*i)->Draw(&shadowShader_, 0);
    }
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular, float constFalloff, float linearFalloff, float quadFalloff)
    : BaseLight{ambient, diffuse, specular, "assets/gl/pointshadow.vs", 
        "assets/gl/pointshadow.fs", "assets/gl/pointshadow.gs"}, position_{position}, 
    constFalloff_{constFalloff}, linearFalloff_{linearFalloff}, quadFalloff_{quadFalloff}
{
    glGenFramebuffers(1, &depthMapFBO_);

    glGenTextures(1, &depthMap_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap_);
    for(unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, 
                GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap_, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float aspect = (float)POINT_SHADOW_WIDTH/(float)POINT_SHADOW_HEIGHT;
    projection_ = glm::perspective(glm::radians(90.0f), aspect, 
            NEAR_SHADOW_CLIPPING_PLANE, FAR_POINT_SHADOW_CLIPPING_PLANE);

}

void PointLight::updateShader(Shader* shader, unsigned int num, unsigned int dirLights)
{
    shader->use();
    std::string name = "pointLights[";
    name += std::to_string(num);
    name += "].";
    shader->setVec3(name + "position", position_);
    shader->setVec3(name + "ambient", ambient_);
    shader->setVec3(name + "diffuse", diffuse_);
    shader->setVec3(name + "specular", specular_);
    shader->setFloat(name + "constant", constFalloff_);
    shader->setFloat(name + "linear", linearFalloff_);
    shader->setFloat(name + "quadratic", quadFalloff_);

    shader->setInt(name + "shadow", num + dirLights);
    glActiveTexture(GL_TEXTURE0 + num + dirLights);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap_);
}


void PointLight::renderShadows(std::vector<BaseObject*> renderables)
{
    glm::mat4 shadowTransforms[6] = 
    {
        projection_ * glm::lookAt(position_, position_ + glm::vec3{1, 0, 0}, 
                glm::vec3{0,-1,0}),
        projection_ * glm::lookAt(position_, position_ + glm::vec3{-1, 0, 0}, 
                glm::vec3{0,-1,0}),
        projection_ * glm::lookAt(position_, position_ + glm::vec3{0, 1, 0}, 
                glm::vec3{0,0,1}),
        projection_ * glm::lookAt(position_, position_ + glm::vec3{0, -1, 0}, 
                glm::vec3{0,0,-1}),
        projection_ * glm::lookAt(position_, position_ + glm::vec3{0, 0, 1}, 
                glm::vec3{0,-1,0}),
        projection_ * glm::lookAt(position_, position_ + glm::vec3{0, 0, -1}, 
                glm::vec3{0,-1,0}),
    };

    glViewport(0,0,POINT_SHADOW_WIDTH, POINT_SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glCullFace(GL_FRONT);

    shadowShader_.use();
    for(int i = 0; i < 6; i++)
    {
        shadowShader_.setMat4("shadowMatrices[" + std::to_string(i) + "]", 
                shadowTransforms[i]);        
    }
    shadowShader_.setFloat("far_plane", FAR_SHADOW_CLIPPING_PLANE);
    shadowShader_.setVec3("lightPos", position_);

    // render all objects
    for(auto i = renderables.begin(); i != renderables.end(); i++)
    {
        (*i)->Draw(&shadowShader_, 0);
    }
    //glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
