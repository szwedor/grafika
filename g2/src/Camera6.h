/*
 * Camera6.h
 *
 *  Created on: Oct 20, 2018
 *      Author: szwed
 */

#ifndef CAMERA6_H_
#define CAMERA6_H_

#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera6: public Camera
{
public:
    Camera6(glm::mat4* viewMatrix): Camera(viewMatrix){}
    virtual void HandleKey(unsigned char input, int x, int y) override;
    virtual void ReleaseKey(unsigned char input, int x, int y) override;
    virtual void UpdateViewMatrix() override;
    virtual void Update() override;
protected:
    virtual void Move(float x, float y, float z) override;
    virtual void AddRotation(int x,int y,int z) override;
private:
    void handleKey();
    glm::vec3 orientation;
    glm::quat camera_quat;
};


#endif /* CAMERA6_H_ */
