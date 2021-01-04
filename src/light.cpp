#include "light.hpp"

BaseLight::BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
    : ambient_{ambient}, diffuse_{diffuse}, specular_{specular}
{
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
    glm::vec3 specular)
    : BaseLight{ambient, diffuse, specular}, direction_{direction}
{

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
