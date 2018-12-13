#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

using namespace std;

class Model
{
  public:
    Model(GLchar *path, GLuint program);
    Model(GLuint program);
    void Draw();
    void Draw(glm::vec3 color);
    void Done();
    void ChangeTexture(GLenum e, GLint i);
    vector<Mesh*> meshes;
    string directory;
    vector<Texture> textures_loaded;
    GLuint programId;

  private:
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);

    Mesh* processMesh(aiMesh *mesh, const aiScene *scene);

    Material Process(aiTextureType texttype, const char *name, aiMaterial *material, const char *ainame, unsigned int type, unsigned int idx);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

    GLint TextureFromFile(const char *path, string directory);
};