/*
 * camera.h
 *
 *  Created on: Oct 18, 2018
 *      Author: szwed
 */

#ifndef CAMERA_H_
#define CAMERA_H_



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(glm::mat4 *viewMatrix) {
        this->viewMatrix = viewMatrix;
        position =glm::vec3(0);

    }
    virtual void HandleKey(unsigned char input, int x, int y)=0;
    virtual void ReleaseKey(unsigned char input, int x, int y)=0;
    virtual void UpdateViewMatrix()=0;
    virtual void Update() = 0;
    glm::vec3 position;
protected:
    virtual void Move(float x, float y, float z)=0;
    virtual void AddRotation(int x, int y,int z)=0;
    glm::mat4 *viewMatrix;
};

#endif /* CAMERA_H_ */
