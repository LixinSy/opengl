#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//����������ƶ�
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f; //����ת����
const GLfloat PITCH = -30.0f;//����ת����
const GLfloat SPEED = 3.0f;//�ƶ��ٶ�
const GLfloat SENSITIVTY = 0.25f;//���������
const GLfloat ZOOM = 45.0f;//����

class Camera
{
public:

	glm::vec3 Position;//�����λ��
	glm::vec3 Front;//���������
	glm::vec3 Up; //����
	glm::vec3 Right; //����
	glm::vec3 WorldUp;//��������

	GLfloat Yaw;//����ת����
	GLfloat Pitch;//����ת����

	GLfloat MovementSpeed;//�ƶ��ٶ�
	GLfloat MouseSensitivity;//���������
	GLfloat Zoom;//����

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	//����������۲����
	glm::mat4 GetViewMatrix();

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void  ProcessKeyboard(Camera_Movement direction, float deltaTime);
	//������껬�ֵ�����
	void ProcessMouseScroll(GLfloat yoffset);

	//�����������������
	void updateCameraVectors();
};
