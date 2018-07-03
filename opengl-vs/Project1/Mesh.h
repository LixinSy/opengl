#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

struct Vertex {  //����
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture { //����
	GLuint id;
	string type;
	aiString path;
};

struct Material // ��ʾ��������
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	vector<Texture> textures;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	~Mesh();
	//��Ⱦmesh
	void Draw();
	void setShader(Shader shader);
	Shader getShader();
	void tranlate();
	void rotate();
	void scale();
	
//private:
	// ��ʼ��VAO,VBO,EBO
	void setupMesh();

	Shader shader;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	GLuint VAO, VBO, EBO;
};

#endif