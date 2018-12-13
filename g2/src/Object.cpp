
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include "Camera6.h"
#include "shader.h"
#include <iostream>
#include <GL/freeglut.h>
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Object::Object(glm::mat4 modelMatrix, Model * model){
    this->model = model;
    this->ModelMatrix = modelMatrix;
	this->color = glm::vec3(-1);
}

Object::Object(glm::mat4 modelMatrix, Model * model, glm::vec3 color){
    this->model = model;
    this->ModelMatrix = modelMatrix;
	this->color = color;
}
void Object::Draw(GLuint ModelMatrixUniformLocation)
{
	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE,
			&ModelMatrix[0][0]);
	if(this->color == glm::vec3(-1))
		model->Draw();
	else 
		model->Draw(this->color);
}
void Object::SetView(glm::mat4 *view){
	for(int i=0;i<this->model->meshes.size();i++)
	this->model->meshes[i]->view = view;
}