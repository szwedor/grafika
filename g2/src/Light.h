 

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <string> 
enum LightType
{
    Point = 0,
    Spot = 1,
    Directional = 2
};


template <typename T>
struct Uniform { 
   public: 
      GLuint Id;
      T Value;
}; 

 class Light
 {
    public:

        static int id;
        Light(GLuint programId, glm::vec3 position, glm::vec3 color, float diffuseintensity = 1.0f, float ambientintensity = 1.0f);
        Uniform<glm::vec3> Position;
        Uniform<glm::vec3> Color;
        Uniform<float> DiffuseIntensity;
        Uniform<float> AmbientIntensity;
        GLuint ProgramId;
        Uniform<LightType> Type;
        Uniform<glm::vec3> Direction;
        Uniform<float> ConeAngle;
        int Id;
        void Draw();
    private:
        void Set(GLuint * id,const char * suffix);
};