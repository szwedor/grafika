
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "SOIL/SOIL.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string.h>
#include <iostream>
#include "Camera6.h"
#include "shader.h"
#include "skybox.h"
#include "Scene.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
// window size 
int windowWidth = 800;
int windowHeight = 600;
float speed = 10;

Shader *shader;
Shader *skyboxShader;
Shader *env;
Skybox *skybox;
Scene * scene;
Scene * envScene;
Camera *camera;
glm::mat4 ProjectionMatrix;
glm::mat4 *ViewMatrix;
GLuint containerTexture, containerSpecularTexture;

glm::mat4 getreflection(glm::mat4 view){
     glm::mat4 viewmat;
    glm::vec3 V = glm::vec3(-view[2][0], -view[2][1], -view[2][2]);
	glm::vec3 R = glm::reflect(V, glm::vec3(0,0,1));

	viewmat = glm::lookAt(scene->mirros[0]->position, scene->mirros[0]->position + R, glm::vec3(0,1,0));
	viewmat = glm::scale(viewmat, glm::vec3(-1,1,1));
    return viewmat;
}

void display_(int t);
void display(void){
    display_(0);
}
void display_(int t)
{   if(t==0){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
     glm::mat4* saveview = ViewMatrix;
            glm::mat4 screenview = glm::mat4(
                glm::vec4(0.507797, 0.321409, 0.799274, 0.000000), 
                glm::vec4(0.000000, 0.879969, -0.475032, 0.000000), 
                glm::vec4(0.681639, 0.347575, 0.643864, 0.000000), 
                glm::vec4(1.169537, -17.002815, -32.770905, 1.000000));
            ViewMatrix = &screenview;
    display_(1);
    ViewMatrix = saveview;
    for( auto const& val : scene->screens ) 
    for(int i=0;i<2;i++)
    {           
                glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, val->model->meshes[0]->materials[i].texture.id+1);
					glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, windowWidth, windowHeight, 0);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                                        glBindTexture(GL_TEXTURE_2D,0);
    }
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
      skyboxShader->Use();
    glm::mat4 skyboxProjection = glm::perspective(glm::radians(35.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(*ViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(skyboxProjection));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniform1f(glGetUniformLocation(skyboxShader->Program, "alpha"), 0.1f);
   glDisable(GL_DEPTH_TEST);
    skybox->Draw(*skyboxShader);
    glEnable(GL_DEPTH_TEST);
       shader->Use();
    shader->Use();
    GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");
	glUniform3fv(viewPosLoc, 1, &camera->position.x);
    glUniform3fv(glGetUniformLocation(shader->Program, "cameraPos"),1, &camera->position.x);
    glm::mat4 view = *ViewMatrix;
    glm::mat4 projection = glm::perspective(glm::radians(35.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
   glUniform1f(glGetUniformLocation(shader->Program, "alpha"), 1.0f);


      glDisable(GL_DEPTH_TEST);
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      glEnable(GL_STENCIL_TEST);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
      scene->DrawMirror();
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glEnable(GL_DEPTH_TEST);

      glStencilFunc(GL_EQUAL, 1, 0xffffffff);  
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glScalef(1.0, -1.0, 1.0);

    //  glEnable(GL_NORMALIZE);
      glCullFace(GL_FRONT);
        
      glm::mat4 viewmat =getreflection(*ViewMatrix);
      glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewmat));
glUniform1f(glGetUniformLocation(shader->Program, "alpha"), 0.1f);

  skyboxShader->Use();
    skyboxProjection = glm::perspective(glm::radians(35.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    skyboxView = glm::mat4(glm::mat3(getreflection(*ViewMatrix)));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(skyboxProjection));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
    glUniform1f(glGetUniformLocation(skyboxShader->Program, "alpha"), 0.1f);
   glDisable(GL_DEPTH_TEST);
    skybox->Draw(*skyboxShader);
       shader->Use();

      scene->Draw();
    glEnable(GL_DEPTH_TEST);
      glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewmat));
     
  env->Use();  
    glUniform3fv(glGetUniformLocation(env->Program, "cameraPos"),1, &camera->position.x);
  
    glm::mat4 pro = glm::perspective(glm::radians(35.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(env->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewmat));
    glUniformMatrix4fv(glGetUniformLocation(env->Program, "projection"), 1, GL_FALSE, glm::value_ptr(pro));
    glUniform1i(glGetUniformLocation(env->Program, "skybox"), 0);
    envScene->Draw(); 
 shader->Use();
   //   glDisable(GL_NORMALIZE);
      glCullFace(GL_BACK);


      glDisable(GL_STENCIL_TEST);
    

     glUniform1f(glGetUniformLocation(shader->Program, "alpha"), 0.3);

    scene->DrawMirror();
glUniform1f(glGetUniformLocation(shader->Program, "alpha"), 1.0f);

     glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
   
    scene->Draw();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glUniform1f(glGetUniformLocation(shader->Program, "alpha"), 0.5f);
    scene->DrawMirror();
    glDisable(GL_BLEND);

    env->Use();  
    glUniform3fv(glGetUniformLocation(env->Program, "cameraPos"),1, &camera->position.x);
    view = *ViewMatrix;
    pro = glm::perspective(glm::radians(35.0f), (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(env->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(env->Program, "projection"), 1, GL_FALSE, glm::value_ptr(pro));
    glUniform1i(glGetUniformLocation(env->Program, "skybox"), 0);
    skybox->Draw(*env,true);
    envScene->Draw();
 
    
	glUseProgram(0);
    if(t==0)
    glutSwapBuffers();
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    ViewMatrix = new glm::mat4();
	*ViewMatrix  = glm::mat4(1);
    camera = new Camera6(ViewMatrix);
    skybox->setup();
    scene = new Scene("config.txt",glGetUniformLocation(shader->Program, "model"),shader->Program);

    envScene = new Scene("config2.txt",glGetUniformLocation(env->Program, "model"),env->Program);


    glClearColor(0.1, 0.1, 0.1, 1.0);
}


void reshape(int width, int height)
{
    windowWidth=width;
    windowHeight=height;
    glutReshapeWindow(windowWidth, windowHeight);
    
    glViewport(0, 0, width, height);
}


void autoRotation(int value)
{
    glutTimerFunc(speed, autoRotation, 0);
    camera->Update();
}



int eventInfo [256];
template<GLenum enumtype, int fval, int sval>
void ChangeTexture(char ind,char const * text)
{
	std::cout<<text<<std::endl;
    if(eventInfo[ind] == fval) eventInfo[ind] = sval;
	else eventInfo[ind] = fval;
	for( auto const& [key, val] : scene->models ) 
    val->ChangeTexture( enumtype,eventInfo[ind]);
}
 
void enableMultisample(int msaa)
{
    if (msaa)
    {
        glEnable(GL_MULTISAMPLE);
        glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

        GLint iMultiSample = 0;
        GLint iNumSamples = 0;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
        glGetIntegerv(GL_SAMPLES, &iNumSamples);
        printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
        printf("MSAA off\n");
    }   
}

int samplerLOD;
void Event(unsigned char c){
	switch(c){
		case '\0':
	scene->randomize();
	std::cout<<"rand"<<std::endl;
	break;
	case '1':
		ChangeTexture<GL_TEXTURE_MAG_FILTER,GL_NEAREST, GL_LINEAR>(c,"wl/wyl liniowy filtr");
		break;
	case '2':
		ChangeTexture<GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST>(c,"filtrowanie trójliniowe");
		break;
	case '3':
	case '4': 
			samplerLOD+=(2*(c-'3')-1);
			std::cout<<samplerLOD<<std::endl;
			for( auto const& [key, val] : scene->models )
				for(GLuint i=0;i< val->textures_loaded.size();i++){
					Texture texture = val->textures_loaded[i];
					glBindTexture(GL_TEXTURE_2D, texture.id);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, samplerLOD*0.5f);
					glBindTexture(GL_TEXTURE_2D, 0);
    }
			break;
	case '5':
			std::cout<<eventInfo[c]<<std::endl;
			enableMultisample(eventInfo[c]);
			eventInfo[c]=(eventInfo[c]+1)%2;

	
		break;
    case ' ':
    
    std::cout<<glm::to_string(*ViewMatrix)<<std::endl;

            break;
	default:
	break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
    Event(key);
   camera->HandleKey(key,x,y);
}
void keyboardUpFunction(unsigned char key, int x, int y)
{
  camera->ReleaseKey(key, x, y);
}


int main(int argc, char **argv)
{
    /*OpenGL*/
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 16);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL|GLUT_MULTISAMPLE);

    glutInitContextVersion(3,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitWindowPosition(300, 300);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("G2");
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        exit(1);
    if (!GLEW_VERSION_2_1)
        exit(1);

    shader = new Shader("./shader/l.vs", "./shader/l.frag");
    skyboxShader = new Shader("./shader/skybox.vs", "./shader/skybox.frag");
    env = new Shader("./shader/env.vs","./shader/env.frag");
    skybox = new Skybox();


    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUpFunction);
    glutTimerFunc(speed, autoRotation, 0);
    glutMainLoop();

    delete shader;
    delete skyboxShader;
    delete skybox;
    delete scene;
    delete camera;
    delete ViewMatrix;
    return 0;
}
