

#include "Model.h"
#include <GL/freeglut.h>
class Object
{
    public:
        Object(glm::mat4 ModelMatrix, Model * model);
        Object(glm::mat4 ModelMatrix, Model * model, glm::vec3 color);
        void Draw(GLuint ModelMatrixUniformLocation);
        void SetView(glm::mat4 *view);
        glm::mat4 ModelMatrix;
        glm::vec3 position;
        Model * model;
    private:
        glm::vec3 color;
};