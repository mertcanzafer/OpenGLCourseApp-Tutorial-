#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace CNS
{
	class Camera
	{
	 public:
		Camera();
		Camera(glm::vec3 startPosition,glm::vec3 startUp, GLfloat startYaw,GLfloat startPitch,GLfloat StartMovementSpeed,GLfloat StartTurnSpeed);
		
		void keyControl(bool* keys,GLfloat deltaTime);
		void mouseControl(GLfloat xChange, GLfloat yChange);

		// Get the camera pos. for specular lighting!
		glm::vec3 GetCameraPosition();
		glm::mat4 calculateViewMatrix();

		~Camera();
	 private:

		 glm::vec3 position; // Position of the camera pos [ x,y,z] in vec3
		 glm::vec3 front;
		 glm::vec3 up;
		 glm::vec3 right;
		 glm::vec3 worldUp;

		 GLfloat yaw; // Looking left + right when turning the viewer's head
		 GLfloat pitch; // Looking up + down when turning the viewer's head

		 GLfloat movementSpeed;
		 GLfloat turnSpeed;

		 void update();
	};
}



