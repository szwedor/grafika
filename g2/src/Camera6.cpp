#include "camera.h"

#include "Camera6.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <atomic>
void Camera6::UpdateViewMatrix() {
	 glm::quat key_quat = glm::quat(orientation);
	  orientation = glm::vec3(0);

	  camera_quat = key_quat * camera_quat;
	  camera_quat = glm::normalize(camera_quat);
	  glm::mat4 rotate = glm::mat4_cast(camera_quat);
  
	  glm::mat4 translate = glm::mat4(1.0f);
	  translate = glm::translate(translate, -position);

	 *this->viewMatrix =  rotate * translate;
}

void Camera6::Move(float x, float y, float z) {
	 glm::mat4 mat = *this->viewMatrix;
	  glm::vec3 forward(mat[0][2], mat[1][2], mat[2][2]);
	  glm::vec3 strafe( mat[0][0], mat[1][0], mat[2][0]);
	  glm::vec3 xd( mat[0][1], mat[1][1], mat[2][1]);

	  const float speed = 0.12f;
	  position += (-y * forward + x * strafe+xd*z) * speed;


}

bool keys [256];
void Camera6::Update(){
	this->handleKey();
	UpdateViewMatrix();
}

void Camera6::AddRotation(int x, int y,int z) {
	orientation.y   += 0.015f*x;
    orientation.x   += 0.015f*y;
	orientation.z   += 0.015f*z;
}
void Camera6::HandleKey(unsigned char input, int x, int y){
	keys[input] = true;
}
void Camera6::ReleaseKey(unsigned char input, int x, int y){
	keys[input] = false;
}
void Camera6::handleKey() {
	float speed = 1.0f;
	int rotate = 1;
	if(keys['w'])
		this->Move(0.0f, speed, 0.0f);
	if(keys['a'])
		this->Move(-speed, 0.0f, 0.0f);
	if(keys['s'])
		this->Move(0.0f, -speed, 0.0f);
	if(keys['d'])
		this->Move(speed, 0.0f, 0.0f);
	if(keys['q'])
		this->Move(0.0f, 0.0f, speed);
	if(keys['e'])
		this->Move(0.0f, 0.0f, -speed);

	if(keys['m'])
		this->AddRotation(0,0,rotate);
	if(keys['n'])
		this->AddRotation(0,0, -rotate);
	if(keys[','])
		this->AddRotation(-rotate,0,0);
	if(keys['.'])
		this->AddRotation(rotate,0,0);
	if(keys['o'])
		this->AddRotation(0,rotate,0);
	if(keys['l'])
		this->AddRotation(0, -rotate,0);
	
	
	//this->UpdateViewMatrix();
}