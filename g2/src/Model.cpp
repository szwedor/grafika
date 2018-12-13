#pragma once
#include "Model.h"
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
#include <assimp/DefaultLogger.hpp>
#include <assimp/Logger.hpp>
#include <time.h>
using namespace std;

Model::Model(GLchar *path, GLuint program)
{
    this->loadModel(path);
    this->programId = program;
}

Model::Model(GLuint program)
{
    this->programId = program;
}
 
void Model::ChangeTexture(GLenum e, GLint val){
	for(GLuint i=0;i< this->textures_loaded.size();i++){
            Texture texture = this->textures_loaded[i];
               glBindTexture(GL_TEXTURE_2D, texture.id);
               glTexParameteri(GL_TEXTURE_2D, e,val);
               glBindTexture(GL_TEXTURE_2D, 0);
    }
}
void Model::Draw()
{
    for (GLuint i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i]->Draw(this->programId);
    }
}
void Model::Draw(glm::vec3 colors)
{
    for (GLuint i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i]->Draw(this->programId,colors);
    }
}
void Model::Done(){
       for (GLuint i = 0; i < this->meshes.size(); i++)
    {
        this->meshes[i]->Done(this->programId);
    }
}
Material  Model::Process(aiTextureType texttype,const char * name,aiMaterial * material,const char* ainame,unsigned int type, unsigned int idx)
{
    vector<Texture> maps = this->loadMaterialTextures(material, texttype, name);
    
    aiColor3D color;
    material->Get(ainame,type,idx, color);
    Material mat;
    mat.hasTexture = maps.size() > 0;
    if(mat.hasTexture)
    mat.texture = maps.front();
    mat.color = glm::vec4(color.r,color.g,color.b,1.0f);
    if(mat.color == glm::vec4(0) && mat.hasTexture)
        mat.color = glm::vec4(1);
    mat.type = name;
    return mat;
}

void Model::loadModel(string path)
{ 
    Assimp::DefaultLogger::get()->setLogSeverity(Assimp::Logger::VERBOSE);
    Assimp::Importer importer;
    
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));
    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

        this->meshes.push_back(this->processMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh *Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Material> materials;
    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if(mesh->mNormals){
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        }
        if (mesh->mTextureCoords[0]) 
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

    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    float shine = 0.0;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        material->Get(AI_MATKEY_SHININESS, shine);
        materials.push_back(Process(aiTextureType_AMBIENT,"ambient",material,AI_MATKEY_COLOR_AMBIENT));
        materials.push_back(Process(aiTextureType_DIFFUSE,"diffuse",material,AI_MATKEY_COLOR_DIFFUSE));
        materials.push_back(Process(aiTextureType_SPECULAR,"specular",material,AI_MATKEY_COLOR_SPECULAR));
        
        
    }
    auto mmesh =  new Mesh(vertices, indices, materials);
    mmesh->specExponent = shine;
    return mmesh;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    vector<Texture> textures;

    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        GLboolean skip = false;

        for (GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if (textures_loaded[j].path == str)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 

                break;
            }
        }

        if (!skip)
        { 
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory + "/Maps");
            texture.type = typeName;
            std::cout << type << std::endl;
            texture.path = str;
            textures.push_back(texture);

            this->textures_loaded.push_back(texture);       }
    }

    return textures;
}

GLint Model::TextureFromFile(const char *path, string directory)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;

    width = height = 128;
    unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}
