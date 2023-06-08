#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "constants.hpp"
#include "assimp/types.h"

struct BoneData
{
    unsigned int id;
    float weight;
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::ivec4 BoneIds;
    glm::vec4 Weights;
};

struct Bone 
{
    glm::mat4 offset;
    unsigned int id;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public: 
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture*> textures_;
    std::vector<Bone> bones_;

    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float shininess_;

    Mesh() = delete;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
        std::vector<Texture*> textures, std::vector<Bone> bones, 
        glm::vec3 diffuse, glm::vec3 specular, float shininess);
    /**
     * Draws this mesh.
     *
     * Note: the textures in the fragment shader must be named in this way:
     *
     * struct Material
     * {
     *    sampler2D texture_diffuse1;
     *    sampler2D texture_diffuse2;
     *    .
     *    .
     *    .
     *    sampler2D texture_specular1;
     *    sampler2D texture_specular2;
     *    .
     *    .
     *    .
     * };
     *
     * uniform Material material;
     */
    void Draw(Shader& shader, int numShadowMaps);

    friend std::ostream& operator<<(std::ostream& os, const Mesh& m);

    /**
     * Binds the vertices and vertex attributes to VAO_.
     *
     * Note: adding vertex attributes would happen in here
     */
    void setupMesh();
    
    unsigned int VAO_, VBO_, EBO_;


};

std::ostream& operator<<(std::ostream& os, const Mesh& m);

#endif
