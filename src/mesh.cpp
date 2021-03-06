#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
    std::vector<Texture*> textures, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : vertices_{vertices}, indices_{indices}, textures_{textures}, diffuse_{diffuse},
        specular_{specular}, shininess_{shininess}
{
    // We call setupmesh later with a gl environment
    // setupMesh();
}


void Mesh::setupMesh()
{
    // Generate our buffers
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    // bind the vertices to the VAO/VBO
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), 
        &vertices_[0], GL_STATIC_DRAW);

    // bind the indices to the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int),
        &indices_[0], GL_STATIC_DRAW);

    // vertex layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*) offsetof(Vertex, Normal));

    // vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*) offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}



void Mesh::Draw(Shader& shader, int numShadowMaps)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    
    // bind the textures
    for(unsigned int i = 0; i < textures_.size(); i++)
    {
        std::cerr << "Here's a texture" << std::endl;
        glActiveTexture(GL_TEXTURE0 + i + numShadowMaps);
        std::string number;
        std::string name = textures_[i]->type;
        if(name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr);
            diffuseNr++;
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr);
            specularNr++;
        }
        shader.setInt(("material." + name + number).c_str(), i + numShadowMaps);
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id);
    }
    shader.setFloat("material.shininess", shininess_);
    shader.setVec3("material.diffuse", diffuse_);
    shader.setVec3("material.specular", specular_);
    
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

std::ostream& operator<<(std::ostream& os, const Mesh& m)
{
    unsigned int i = 0;
    for(auto iter = m.vertices_.begin(); iter != m.vertices_.end(); iter++)
    {
        os << i << ":";
        os << iter->Position.x << " " << iter->Position.y << " " << iter->Position.z << ":";
        os << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << ":" 
            << std::endl;
        i++;
    }
    os << std::endl;
    auto iter = m.indices_.begin(); 
    while(iter != m.indices_.end())
    {
        os << *iter << ", ";
        iter++;
        os << *iter << ", ";
        iter++;
        os << *iter;
        iter++;
        os << std::endl;
    }
    os << std::endl;
    
    return os;
}
