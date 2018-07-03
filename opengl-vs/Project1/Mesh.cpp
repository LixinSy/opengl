

#include "Mesh.h"

// ���캯��
Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	// ��ʼ��VAO,VBO,EBO
	this->setupMesh();
}

//��������
Mesh::~Mesh()
{

}

//��Ⱦmesh
void Mesh::Draw()
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		// �����������к� (N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // ��GLuin���뵽string stream
		else if (name == "texture_specular")
			ss << specularNr++; // ��GLuin���뵽string stream
		number = ss.str();

		// ������
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		// ����ɫ����������
		glUniform1i(glGetUniformLocation(shader.getProgram(), (name + number).c_str()), i);
	}

	//glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 16.0f);

	// ����Mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);

}

// ��ʼ��VAO,VBO,EBO
void Mesh::setupMesh()
{
	// ����VAO,VBO,EBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// ��VBO��������
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	// ��EBO��������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// ���ö�������
	// ����λ��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// ������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// ��������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void Mesh::setShader(Shader shader)
{
	this->shader = shader;
}
Shader Mesh::getShader()
{
	return shader;
}


