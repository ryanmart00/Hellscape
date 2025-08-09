#include "model.hpp"

#ifndef STB_INCLUDE_GUARD
#define STB_INCLUDE_GUARD
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif
#include "constants.hpp"
#include "glad/gl.h"

Model::Model(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        std::exit(1);
    }
    directory_ = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    // We initialize the rest when we have a GL context
    import.FreeScene();
}

Model::~Model()
{
    for(auto i = meshes_.begin(); i != meshes_.end(); i++)
    {
        glDeleteVertexArrays(1, &(i->VAO_));
        glDeleteBuffers(1, &(i->VBO_));
        glDeleteBuffers(1, &(i->EBO_));
    }

    auto i = texturesLoaded_.begin();
    while(i != texturesLoaded_.end())
    {
        Texture* t = *i;
        i = texturesLoaded_.erase(i);
        glDeleteTextures(1, &t->id);
        delete t;
    }
    
}

void Model::initializeGL()
{
    for(auto i = texturesLoaded_.begin(); i != texturesLoaded_.end(); ++i)
    {
        (*i)->id = TextureFromFile((*i)->path.c_str(), directory_);
    }
    for(auto i = meshes_.begin(); i != meshes_.end(); ++i)
    {
        i->setupMesh();
    }
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process meshes
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(processMesh(mesh, scene));
    }
    // process child nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    std::vector<Bone> bones;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions
        glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.Position = vec;

        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex.Normal = vec;
        glm::vec4 zero = glm::vec4{0};
        vertex.BoneIdsA = zero;
        vertex.BoneIdsB = zero;
        vertex.WeightsA = zero;
        vertex.WeightsB = zero;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process bones
    if(mesh->HasBones())
    {
#ifdef DEBUG
        if(mesh->mNumBones > MAX_BONES)
        {
            std::cerr << "Mesh has too many bones!"  << std::endl;
        }
#endif 
        std::vector<unsigned int> vertexCounts(vertices.size()); 
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            vertexCounts[i] = 0;
        }
        for(unsigned int i = 0; i < mesh->mNumBones; i++)
        {
            bones.push_back(
                Bone
                {
                    convert(mesh->mBones[i]->mOffsetMatrix),
                    i,
                    mesh->mBones[i]->mName.C_Str()
                });
            std::cout << mesh->mBones[i]->mNumWeights << std::endl;
            for(unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
            {
                float w = mesh->mBones[i]->mWeights[j].mWeight;
                if(w != 0.0f)
                {
                    unsigned int v = mesh->mBones[i]->mWeights[j].mVertexId;
                    Vertex& vert = vertices[v];
                    unsigned int k = vertexCounts[v];
                    vertexCounts[v]++;
                    if(k < 4)
                    {
                        vert.BoneIdsA[k] = i;
                        vert.WeightsA[k] = w;
                    }
                    else if(k < 8)
                    {
                        vert.BoneIdsB[k - 4] = i;
                        vert.WeightsB[k - 4] = w;
                    }
                    else 
                    {
                        std::cerr << "Too many bones!!" << std::endl;
                        std::exit(1);
                    }
                }
            }

        }
        for(auto v = vertices.begin(); v != vertices.end(); v++)
        {
            float l = sum(v->WeightsA + v->WeightsB);
            v->WeightsA = v->WeightsA / l;
            v->WeightsB = v->WeightsB / l;
        }
    }


    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess = 32;
    // process material
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color;
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        diffuse.x = color.r;
        diffuse.y = color.g;
        diffuse.z = color.b;

//        std::cout << diffuse.x << " " << diffuse.y << " " << diffuse.z << std::endl;

        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        specular.x = color.r;
        specular.y = color.g;
        specular.z = color.b;

        material->Get(AI_MATKEY_SHININESS, shininess);
        
        
        std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture*> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    Mesh m{vertices, indices, textures, bones, diffuse, specular, shininess};

    return m;

}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
    std::string typeName)
{
    std::vector<Texture*> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << "    " << str.C_Str() << std::endl;
        bool skip = false;
        for(unsigned int j = 0; j < texturesLoaded_.size(); j++)
        {
            if(std::strcmp(texturesLoaded_[j]->path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded_[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture* texture = new Texture();
//          We load the texture id later when we have a gl environment
//            texture.id = TextureFromFile(str.C_Str(), directory_);
            texture->type = typeName;
            texture->path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded_.push_back(texture);
        }   
    }
    return textures;
}

void Model::Draw(Shader* shader, int numShadowMaps) 
{
    for(unsigned int i = 0; i < meshes_.size(); i++) 
    {
        meshes_[i].Draw(shader, numShadowMaps);
    }
}

unsigned int TextureFromFile(const char *path, const std::string& directory, bool)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else //nrComponents == 4
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::ostream& operator<<(std::ostream& os, const Model& m)
{
    for(auto iter = m.meshes_.cbegin(); iter != m.meshes_.cend(); iter++)
    {
        os << *iter << std::endl;
    }
    return os;
}

