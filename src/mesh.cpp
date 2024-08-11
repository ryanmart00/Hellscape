#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
    std::vector<Texture*> textures, std::vector<Bone> bones, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : vertices_{vertices}, indices_{indices}, textures_{textures}, bones_{bones}, diffuse_{diffuse},
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

    // bone ids (for now we have 8 but could increase)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_INT, GL_FALSE, sizeof(Vertex), 
            (void*) offsetof(Vertex, BoneIdsA));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, sizeof(Vertex), 
            (void*) offsetof(Vertex, BoneIdsB));


    // bone weights
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*) offsetof(Vertex, WeightsA));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void*) offsetof(Vertex, WeightsB));

    glBindVertexArray(0);
}



void Mesh::Draw(Shader* shader, int numShadowMaps)
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
        shader->setInt(("material." + name + number).c_str(), i + numShadowMaps);
        glBindTexture(GL_TEXTURE_2D, textures_[i]->id);
    }
    shader->setFloat("material.shininess", shininess_);
    shader->setVec3("material.diffuse", diffuse_);
    shader->setVec3("material.specular", specular_);
    
    glActiveTexture(GL_TEXTURE0);

    // set bone offsets
    for(int i = 0; i < bones_.size(); i++)
    {
        shader->setMat4("bones[" + std::to_string(i) + "]", bones_[i].offset);
    }

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
        os << iter->Position.x << " " << iter->Position.y << " " << iter->Position.z << " : ";
        os << iter->Normal.x << " " << iter->Normal.y << " " << iter->Normal.z << " : ";
        os << iter->BoneIdsA.x << "-" << iter->WeightsA.x << " " 
            << iter->BoneIdsA.y << "-" << iter->WeightsA.y << " " 
            << iter->BoneIdsA.z << "-" << iter->WeightsA.z << " " 
            << iter->BoneIdsA.w << "-" << iter->WeightsA.w << " : ";
        os << iter->BoneIdsB.x << "-" << iter->WeightsB.x << " " 
            << iter->BoneIdsB.y << "-" << iter->WeightsB.y << " " 
            << iter->BoneIdsB.z << "-" << iter->WeightsB.z << " " 
            << iter->BoneIdsB.w << "-" << iter->WeightsB.w << " : ";
        os  << iter->WeightsA.x + iter->WeightsA.y + iter->WeightsA.z + iter->WeightsA.w
            + iter->WeightsB.x + iter->WeightsB.y + iter->WeightsB.z + iter->WeightsB.w
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
