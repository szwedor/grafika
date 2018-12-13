
#include <vector>
#include "Object.h"
#include "Model.h"
#include <GL/freeglut.h>
using namespace std;

class Scene
{
public:
    Scene(const char * filename,GLuint modelMatrixUniformLocation, GLuint programId);
    void Draw();
    void DrawMirror();
    void DrawScreen(glm::mat4 *view);
    void randomize();
    map<string, Model*> models;
    vector<Object*> mirros;
    vector<Object*> screens;
private:
    void LoadModels();
    void Set();

    vector<Object*> objects;
    vector<Object*> envs;
    GLuint ModelMatrixUniformLocation;
    GLuint Program;
};