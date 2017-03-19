#pragma once

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "GT_Mesh.h"

GLint textureFromFile(const char* texturePath, string directory, bool gamma = false);

class GT_Model
{

public:

    vector<Texture> textures_loaded;
    vector<GT_Mesh> meshes;
    string directory;
    bool gammaCorrection;

    Model(string const & path, bool gamma = false)
        : gammaCorrection(gamma)
    {
        this->loadModel(path);
    }

    // Draws the model and thus all its meshes
    void Draw(GT_Shader shader_)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {
            this->meshes[i].Draw(shader_);
        }
    }

private:

    void loadModel(string path)
    {
        // Read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        // CHeck for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) // if is Not Zero
        {
            std::cout<< "ERROR::ASSIMP:: "<<importer.GetErrorString()<<std::endl;
            return;
        }

        // Retrieve the directory path of the filepath
        this->directory = path.substr(0, path.find_last_of('/'));

        // Process ASSIMP's root node recursively
        this->processNode(scene->mRootNode, scene);


    }
};
