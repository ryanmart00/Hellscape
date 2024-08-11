#ifndef MODEL_HPP_INCLUDED
#define MODEL_HPP_INCLUDED

#include <string>
#include <vector>

#include "shader.hpp"
#include "mesh.hpp"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

unsigned int TextureFromFile(const char* path, const std::string &directory, 
    bool gamma = false);

class Model
{
public:
    Model() = delete;
    Model(std::string path);
    ~Model();

    void initializeGL();
    
    void Draw(Shader* shader, int numShadowMaps);

    friend std::ostream& operator<<(std::ostream& os, const Model& m);

public:
    
    std::vector<Mesh> meshes_;
    std::string directory_;
    std::vector<Texture*> texturesLoaded_;
    
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);

};

std::ostream& operator<<(std::ostream& os, const Model& m);

#endif
