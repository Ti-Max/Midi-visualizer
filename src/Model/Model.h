#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<vector>
#include <string>
#include"../../libs/others/stb_image.h"

#include "Shader/Shader.h"
#include "VAO/VAO.h"
//#include "Object/Texture.h"

using namespace std;
class Model
{
public:
    Model();
    ~Model();
	Model(const std::string& path);

    //vector<Texture> textures_loaded;
    vector<GL::VAO*> meshes;
    void loadModel(string path);
private:
    /*  Данные модели  */
    string directory;

    /*  Методы   */
    void processNode(aiNode* node, const aiScene* scene);
    GL::VAO* processMesh(aiMesh* mesh, const aiScene* scene);/*
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(string& path, const string& directory);*/

};

