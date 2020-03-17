#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "shader.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
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
    std::vector<Texture> textures_;

    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float shininess_;

    Mesh() = delete;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, 
        std::vector<Texture> textures, glm::vec3 diffuse, glm::vec3 specular, float shininess);
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
    void Draw(Shader& shader);

    friend std::ostream& operator<<(std::ostream& os, const Mesh& m);
    
private:
    unsigned int VAO_, VBO_, EBO_;

    /**
     * Binds the vertices and vertex attributes to VAO_.
     *
     * Note: adding vertex attributes would happen in here
     */
    void setupMesh();

};

std::ostream& operator<<(std::ostream& os, const Mesh& m);

#endif
