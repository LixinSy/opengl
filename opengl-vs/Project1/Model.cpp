
#include <glm/glm.hpp>
//#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"

// ���캯��
Model::Model(GLchar* path, Shader shader)
{
	this->loadModel(path);
	this->shader = shader;
}

// ��Ⱦģ��
void Model::Draw()
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].Draw();
	}
}

// ����ģ��
void Model::loadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// �Ƿ���سɹ�
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	
	this->directory = path.substr(0, path.find_last_of('/'));
	//cout << path << endl;
	//cout << directory << endl;
	this->processNode(scene->mRootNode, scene);

	float x = 0, y = 0, z = 0, num = 0;
	for (int i = 0; i < meshes.size(); i++){
		for (int j = 0; j < meshes[i].vertices.size(); j++){
			x += meshes[i].vertices[j].position.x;
			y += meshes[i].vertices[j].position.y;
			z += meshes[i].vertices[j].position.z;
			num ++;
		}
	}
	x = x / num;
	y = y / num;
	z = z / num;
	std::cout << path << ": " << x << " " << y << " " << z << endl;
}

//����ڵ�
void Model::processNode(aiNode* node, const aiScene* scene) {
	//����ڵ������������Ϣ
	for (int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}
	//�����е��ӽڵ�����ͬ����
	for (int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

//��������
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	
	// ��ö�������
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		// ��������
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// ������
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		// ��������
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ��ö�������EBO
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// ��ò��ʣ�����
	if (mesh->mMaterialIndex >= 0)
	{
		//cout <<mesh->mName.C_Str()<< "�в���" << endl;
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. Diffuse maps
		vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	else
	{
		cout << mesh->mName.C_Str() << "û�в��ʣ���������������������������������" << endl;
	}
/*	for (int i = 0; i < vertices.size(); i++){
		Vertex vertex = vertices[i];
		std::cout <<i+1<<": "<< vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ", "
			//<< vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << " "
			<< vertex.texCoords.x << ", " << vertex.texCoords.y <<", "<< endl;
	}
	for (int i = 0; i<indices.size(); i++)
	{
		std::cout << indices[i]<<", ";
	}
	std::cout << endl << vertices.size() << " " << indices.size();
*/
	Mesh myMesh(vertices, indices, textures);
	myMesh.setShader(this->shader);
	return myMesh;
}

// ��������
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // �������û�б����ع�������֮
			Texture texture;
			texture.id = this->TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // ��ӵ������б� textures
		}
	}
	return textures;
}

GLint Model::TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "�������ʧ�ܣ�·����" << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


