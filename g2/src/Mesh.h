#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;
};
struct Material
{
	string type;
	glm::vec4 color;
	GLuint colorId;
	Texture texture;
	GLuint textureId;
	bool hasTexture;
	GLuint hasTextureId;
};
class Mesh
{
  public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Material> materials;
	float specExponent;
	glm::mat4 *view;
	int x,y,z;
	Mesh(vector<Vertex> vertices,
		 vector<GLuint> indices,
		 vector<Material> materials,int x=1,int y=1,int z=1)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->materials = materials;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	virtual void Draw(GLuint programId)
	{
		for (GLuint i = 0; i < this->materials.size(); i++)
		{
			Material mat = this->materials[i];
			glUniform4fv(mat.colorId, 1, &mat.color.x);
		}
		draw(programId);
	}
	virtual void Draw(GLuint programId, glm::vec3 colors)
	{
		for (GLuint i = 0; i < this->materials.size(); i++)
		{
			Material mat = this->materials[i];
			glUniform4fv(mat.colorId, 1, &colors.x);
		}
		draw(programId);
	}
	virtual void draw(GLuint programId)
	{
		glUniform1f(glGetUniformLocation(programId, "material_specExponent"), specExponent);

		for (GLuint i = 0; i < this->materials.size(); i++)
		{
			Material mat = this->materials[i];

			//glUniform3fv(mat.colorId, 1, &mat.color.x);
			if (mat.hasTexture)
			{
				//glActiveTexture(GL_TEXTURE0 + i+1);
				glUniform1i(mat.textureId, i);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, mat.texture.id);
			}
			glUniform1i(mat.hasTextureId, mat.hasTexture);
		}

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE0, 0);
	}
	GLuint VAO, VBO, EBO;
	void Done(GLuint programId)
	{
		for (GLuint i = 0; i < this->materials.size(); i++)
		{
			string name = this->materials[i].type;

			std::stringstream s;
			s << name << ".color";
			std::string colorName = s.str();
			this->materials[i].colorId = glGetUniformLocation(programId, colorName.c_str());

			if (this->materials[i].hasTexture)
			{
				std::stringstream s2;
				s2 << name << ".texture";
				std::string texName = s2.str();
				this->materials[i].textureId = glGetUniformLocation(programId, texName.c_str());
			}
			std::stringstream sb;
			sb << name << ".hasTexture";
			std::string boolName = sb.str();
			this->materials[i].hasTextureId = glGetUniformLocation(programId, boolName.c_str());
		}
		this->setupMesh();
	}

  private:
	void setupMesh()
	{
		VAO = VBO = EBO = 0;
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenBuffers(2, &this->VBO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
					 &this->vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  (GLvoid *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  (GLvoid *)offsetof(Vertex, Normal));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					 this->indices.size() * sizeof(GLuint), &this->indices[0],
					 GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
							  (GLvoid *)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};
