#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <string>
#include <sstream>
#include "Light.h"
#include <iostream>
int Light::id;

Light::Light(GLuint programId, glm::vec3 position, glm::vec3 color, float diffuseintensity, float ambientintensity)
{
    ProgramId = programId;
    Position.Value = position;
    Color.Value = color;

    DiffuseIntensity.Value = diffuseintensity;
    AmbientIntensity.Value = ambientintensity;

    Type.Value = LightType(Point);
    Direction.Value = glm::vec3(0, 0, 1);
    ConeAngle.Value = 15.0f;
    Id = id;

    Set(&Position.Id, "position");
    Set(&Color.Id, "color");
    Set(&Direction.Id, "direction");

    Set(&DiffuseIntensity.Id, "diffuseIntensity");
    Set(&AmbientIntensity.Id, "ambientIntensity");
    Set(&ConeAngle.Id, "coneAngle");
    
    Set(&Type.Id, "type");

    id++;
}

void Light::Draw()
{
    glUniform3fv(Position.Id, 1, &Position.Value.x);
    glUniform3fv(Color.Id, 1, &Color.Value.x);
    glUniform3fv(Direction.Id, 1 ,&Direction.Value.x);

    glUniform1f(DiffuseIntensity.Id, DiffuseIntensity.Value);
    glUniform1f(AmbientIntensity.Id, AmbientIntensity.Value);
    glUniform1f(ConeAngle.Id,  ConeAngle.Value);

    glUniform1f(Type.Id,(int) Type.Value);
}
void Light::Set(GLuint *id, const char *suffix)
{
    std::stringstream s;
    s << "lights[" << Id << "]." << suffix;
    std::string str = s.str();
    *id = glGetUniformLocation(ProgramId, str.c_str());
}