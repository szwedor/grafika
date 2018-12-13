#include "Scene.h"
#include "Model.h"
#include "Light.h"
#include <GL/freeglut.h>
#include <fstream>
#include <glm/glm.hpp>					//vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>			//value_ptr
#include <random>
#include "perlin.h"
vector<Material> GetMaterials(glm::vec3 color)
{
	const char *materials[3] = {"ambient", "diffuse", "specular"};
	vector<Material> vec;
	for (int i = 0; i < 3; i++)
	{
		Material m;
		m.color = glm::vec4(color,1.0f);
		m.type = materials[i];
		m.hasTexture = false;
		
		vec.push_back(m);
	}
	return vec;
}
vector<Light> lights;


void CalculateNormals(Mesh *mesh)
{
	vector<glm::vec3> normals;
	for (int i = 0; i < mesh->vertices.size(); i++)
		mesh->vertices[i].Normal = glm::vec3(0);

	for (unsigned int i = 0; i < mesh->indices.size(); i += 3)
	{
		unsigned int Index0 = mesh->indices[i];
		unsigned int Index1 = mesh->indices[i + 1];
		unsigned int Index2 = mesh->indices[i + 2];
		glm::vec3 v1 = mesh->vertices[Index1].Position - mesh->vertices[Index0].Position;
		glm::vec3 v2 = mesh->vertices[Index2].Position - mesh->vertices[Index0].Position;
		glm::vec3 Normal = glm::normalize(glm::cross(v1, v2));

		mesh->vertices[Index0].Normal += Normal;
		mesh->vertices[Index1].Normal += Normal;
		mesh->vertices[Index2].Normal += Normal;
	}

	for (unsigned int i = 0; i < mesh->vertices.size(); i++)
	{
		mesh->vertices[i].Normal = glm::normalize(mesh->vertices[i].Normal);
	}
}

GLint TextureFromFile(const char *path, string directory, int * width, int * height)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);

    unsigned char *image = SOIL_load_image(filename.c_str(), width, height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}

Mesh *GetCircle(int latitude)
{
std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

Vertex v1;
	v1.Position = glm::vec3(0, 0, 0);
	v1.TexCoords =glm::vec2(0.5,0.5);
	vertices.push_back(v1);
	for (uint32_t j = 0; j < latitude ; ++j)
	{
		double const polar = 2*M_PI * double(j + 1) / double(latitude);
		double const sp = std::sin(polar);
		double const cp = std::cos(polar);
			float const x = sp*0.6 ;
			float const y = cp;
			float const z = 0;
			Vertex v;
			v.Position = glm::vec3(x, y, z);
			v.TexCoords = glm::vec2((x+1)/2,(y+1)/2);
			vertices.push_back(v);
	}

		
	for (uint32_t i = 0; i < latitude; ++i)
	{
		uint32_t const a = i + 1;
		uint32_t const b = (i + 1) % latitude + 1;
		indices.emplace_back(0);
		indices.emplace_back(b);
		indices.emplace_back(a);
	}


	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].Normal =glm::normalize(vertices[i].Position);
	}

const char *materials[3] = {"ambient", "diffuse", "specular"};
	vector<Material> vec;
	for (int i = 0; i < 3; i++)
	{
		Material m;
		m.color = glm::vec4(1.0f,1.0f,1.0f,0.5f);
		m.type = materials[i];
		m.hasTexture = true;
		int w, h;
		m.texture.id = TextureFromFile("th.jpg",".",&w,&h);
		vec.push_back(m);
	}

	return new Mesh(vertices, indices, vec);
}


Mesh *GetScreen(float width, float height){
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Vertex v1,v2,v3,v4;
	v1.Position = glm::vec3(width,0,height);
	v1.Normal = glm::vec3(0,1,0);
	v2.Position = glm::vec3(-width,0,height);
	v2.Normal = glm::vec3(0,1,0);
	v3.Position = glm::vec3(-width,0,-height);
	v3.Normal = glm::vec3(0,1,0);
	v4.Position = glm::vec3(width,0,-height);
	v4.Normal = glm::vec3(0,1,0);
	v1.TexCoords = glm::vec2(0,0);
	v2.TexCoords = glm::vec2(0,1);
	v3.TexCoords = glm::vec2(1,1);
	v4.TexCoords = glm::vec2(1,0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(0);
	const char *materials[3] = {"ambient", "diffuse", "specular"};
	vector<Material> vec;
	int w,h;
	for (int i = 0; i < 3; i++)
	{
		Material m;
		m.color = glm::vec4(1.0f,1.0f,1.0f,0.5f);
		m.type = materials[i];
		m.hasTexture = true;
		
		m.texture.id = TextureFromFile("th.jpg",".",&w,&h);
		vec.push_back(m);
	}

	return new Mesh(vertices, indices, vec,w,h);
}

Mesh *GetCylinder(int longitude)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Vertex v1;
	v1.Position = glm::vec3(0, 1, 0);
	vertices.push_back(v1);
	for (uint32_t i = 0; i < longitude; ++i)
	{
		double const azimuth = 2.0 * M_PI * double(i) / double(longitude);
		double const sa = std::sin(azimuth);
		double const ca = std::cos(azimuth);
		float const x = ca;
		float const y = sa;
		Vertex v;
		v.Position = glm::vec3(x, 1, y);
		vertices.push_back(v);
		Vertex v2;
		v2.Position = glm::vec3(x, -1, y);
		vertices.push_back(v2);
	}
	Vertex ve;
	ve.Position = glm::vec3(0, -1, 0);
	vertices.push_back(ve);
	for (int i = 0; i < vertices.size(); i++)
		vertices[i].TexCoords = glm::vec2(0.4, 0.4);

	for (uint32_t i = 0; i < longitude; ++i)
	{
		uint32_t const a = i;
		uint32_t const b = (i + 1) % longitude;
		;

		indices.emplace_back(0);
		indices.emplace_back(2 * b + 1);
		indices.emplace_back(2 * a + 1);

		indices.emplace_back(2 * a + 1);
		indices.emplace_back(2 * b + 1);
		indices.emplace_back(2 * b + 2);

		indices.emplace_back(2 * b + 2);
		indices.emplace_back(2 * a + 2);
		indices.emplace_back(2 * a + 1);

		indices.emplace_back(vertices.size() - 1);
		indices.emplace_back(2 * a + 2);
		indices.emplace_back(2 * b + 2);
	}

	vector<Material> materials;

	return new Mesh(vertices, indices, GetMaterials(glm::vec3(0.66)));
}
Mesh *GetSphere(int latitude, int longitude)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Vertex v1;
	v1.Position = glm::vec3(0, 1, 0);
	
	vertices.push_back(v1);
	for (uint32_t j = 0; j < latitude - 1; ++j)
	{
		double const polar = M_PI * double(j + 1) / double(latitude);
		double const sp = std::sin(polar);
		double const cp = std::cos(polar);
		for (uint32_t i = 0; i < longitude; ++i)
		{
			double const azimuth = 2.0 * M_PI * double(i) / double(longitude);
			double const sa = std::sin(azimuth);
			double const ca = std::cos(azimuth);
			float x_cor = i * cp;
			float y_cor = j;
			float const x = sp * ca;
			float const y = cp;
			float const z = sp * sa;
			Vertex v;
			v.Position = glm::vec3(x, y, z);
			v.Normal =glm::normalize(v.Position);
			v.Position = v.Position * (1-
			perlin::noise(x_cor,y_cor)/2);

			vertices.push_back(v);
		}
	}
	Vertex ve;
	ve.Position = glm::vec3(0, -1, 0);
	vertices.push_back(ve);
	for (int i = 0; i < vertices.size(); i++)
		vertices[i].TexCoords = glm::vec2(i / (float)vertices.size());
	for (uint32_t i = 0; i < longitude; ++i)
	{
		uint32_t const a = i + 1;
		uint32_t const b = (i + 1) % longitude + 1;
		indices.emplace_back(0);
		indices.emplace_back(b);
		indices.emplace_back(a);
	}

	for (uint32_t j = 0; j < latitude - 2; ++j)
	{
		uint32_t aStart = j * longitude + 1;
		uint32_t bStart = (j + 1) * longitude + 1;
		for (uint32_t i = 0; i < longitude; ++i)
		{
			const uint32_t a = aStart + i;
			const uint32_t a1 = aStart + (i + 1) % longitude;
			const uint32_t b = bStart + i;
			const uint32_t b1 = bStart + (i + 1) % longitude;

			indices.emplace_back(a);
			indices.emplace_back(a1);
			indices.emplace_back(b1);
			indices.emplace_back(a);
			indices.emplace_back(b1);
			indices.emplace_back(b);
		}
	}

	for (uint32_t i = 0; i < longitude; ++i)
	{
		uint32_t const a = i + longitude * (latitude - 2) + 1;
		uint32_t const b = (i + 1) % longitude + longitude * (latitude - 2) + 1;
		indices.emplace_back(vertices.size() - 1);
		indices.emplace_back(a);
		indices.emplace_back(b);
	}

	return new Mesh(vertices, indices, GetMaterials(glm::vec3(0.44)));
}
Scene::Scene(const char * filename,GLuint modelMatrixUniformId, GLuint programId)
{
	this->ModelMatrixUniformLocation = modelMatrixUniformId;
	this->Program = programId;

	std::ifstream infile(filename);
	Model *m;
	if (infile.fail())
	{
		std::cout << "error" << endl;
	}
	std::string path, type;
	infile >> path;
	while (!infile.eof())
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		glm::vec4 color = glm::vec4(0);
		glm::vec3 position;
		do
		{
			infile >> type;
			if (type == "position")
			{
				float pos[3];
				for (int i = 0; i < 3; i++)
					infile >> pos[i];
				position = glm::vec3(pos[0], pos[1], pos[2]);
				modelMatrix = glm::translate(modelMatrix, position);
			}
			else if (type == "rotation")
			{
				float rot[4];
				for (int i = 0; i < 4; i++)
					infile >> rot[i];
				modelMatrix = glm::rotate(modelMatrix, rot[3], glm::vec3(rot[0], rot[1], rot[2]));
			}
			else if (type == "color")
			{
				float colorArr[3];
				for (int i = 0; i < 3; i++)
					infile >> colorArr[i];
				color = glm::vec4(colorArr[0], colorArr[1], colorArr[2],1.0f);
			}
			else if (type == "scale")
			{
				float scale[3];
				for (int i = 0; i < 3; i++)
					infile >> scale[i];
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scale[0], scale[1], scale[2]));
			}
		} while (type != "end");

		auto iter = this->models.find(path);
		if (iter != this->models.end())
		{
			m = iter->second;
		}
		else
		{
			if (path == "cylinder")
			{
				m = new Model(programId);
				Mesh *mesh = GetCylinder(40);
				CalculateNormals(mesh);
				mesh->specExponent = 5;
				if (color != glm::vec4(0))
					for (int i = 0; i < mesh->materials.size(); i++)
						mesh->materials[i].color = color;
				m->meshes.push_back(mesh);
			}
			else if (path == "sphere")
			{
				m = new Model(programId);
				Mesh *mesh = GetSphere(40,40);
				mesh->specExponent = 5;
				//CalculateNormals(mesh);
				if (color != glm::vec4(0))
					for (int i = 0; i < mesh->materials.size(); i++)
						mesh->materials[i].color = color;
				m->meshes.push_back(mesh);
			}
			else if (path == "mirror")
			{
				m = new Model(programId);
				Mesh *mesh = GetCircle(40);
				mesh->specExponent = 5;
				//CalculateNormals(mesh);
				if (color != glm::vec4(0))
					for (int i = 0; i < mesh->materials.size(); i++)
						mesh->materials[i].color = color;
				m->meshes.push_back(mesh);
			}else if (path == "screen")
			{
				m = new Model(programId);
				Mesh *mesh = GetScreen(5,2);
				mesh->specExponent = 5;
				//CalculateNormals(mesh);
				if (color != glm::vec4(0))
					for (int i = 0; i < mesh->materials.size(); i++)
						mesh->materials[i].color = color;
				m->meshes.push_back(mesh);
			}
			else
			{

				m = new Model((GLchar *)path.c_str(), programId);

				///		for(int i=0;i<m->meshes.size();i++)
				//CalculateNormals(&m->meshes[i]);
			}
			m->Done();
			this->models.insert(std::pair<string, Model *>(path, m));
		}

		 
		Object *o; 
		
		if (color != glm::vec4(0))
			o = new Object(modelMatrix, m,color);
		else
			o = new Object(modelMatrix, m);
		
		o->position = position;
		if(path=="mirror")
		this->mirros.push_back(o);
		else if(path=="screen")
		this->screens.push_back(o);
		else
		this->objects.push_back(o);

		infile >> path;
	}
//	this->envs.push_back(dragon);


	infile.close();

	Light *light = new Light(programId, glm::vec3(0, 0, 0), glm::vec3(1),0,0);
	light->AmbientIntensity.Value = 0.3;
	light->DiffuseIntensity.Value = 0.2;
	light->Direction.Value = glm::vec3(1,1,1);
	light->Color.Value = glm::vec3(0,1,1);
    light->Type.Value = LightType(Directional);
	lights.push_back(*light);  
	
	light = new Light(programId, glm::vec3(5,5,5), glm::vec3(1),0,0);
	light->AmbientIntensity.Value = 0;
	light->DiffuseIntensity.Value = 1;
	light->Type.Value = LightType(Spot);
	light->Direction.Value = -glm::normalize(glm::vec3(5,-5,5));
	light->ConeAngle.Value=30;
	light->Color.Value = glm::vec3(1,0,0);
	lights.push_back(*light);

	light = new Light(programId, glm::vec3(5,5,5), glm::vec3(1),0,0);
	light->AmbientIntensity.Value = 0;
	light->DiffuseIntensity.Value = 1;
	light->Type.Value = LightType(Spot);
	light->Direction.Value = -glm::normalize(glm::vec3(0,-5,10));
	light->ConeAngle.Value=40;
	light->Color.Value = glm::vec3(1,0,1);
	lights.push_back(*light);
}

void Scene::Draw()
{
	for (int i = 0; i < this->objects.size(); i++)
		this->objects[i]->Draw(this->ModelMatrixUniformLocation);
	for (int i = 0; i < this->screens.size(); i++)
		this->screens[i]->Draw(this->ModelMatrixUniformLocation);

	for (int i = 0; i < lights.size(); i++)
		lights[i].Draw();
}
void Scene::DrawMirror(){
	for (int i = 0; i < this->mirros.size(); i++)
			this->mirros[i]->Draw(this->ModelMatrixUniformLocation);
	for (int i = 0; i < lights.size(); i++)
		lights[i].Draw();
}

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

void Scene::randomize(){


	lights[lights.size()-1].Color.Value =  glm::vec3(dis(gen),dis(gen),dis(gen));
}