#include "Camera.h"
using namespace CNS;

Camera::Camera(){}

Camera::Camera
(
	glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat StartMovementSpeed, GLfloat StartTurnSpeed
):
	position{startPosition},worldUp{startUp},yaw{startYaw},pitch{startPitch},
	front{glm::vec3(0.0f,0.0f,-1.0f)},movementSpeed{StartMovementSpeed}, turnSpeed{StartTurnSpeed}
{
	update();
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front,worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl
(
	bool* keys, GLfloat deltaTime
)
{
	GLfloat velocity = movementSpeed * deltaTime;

	// W is moving forward
  if(keys[GLFW_KEY_W])
  {
	  position += front * velocity;	
  }
  // S is moving backward
  else if (keys[GLFW_KEY_S])
  {
	  position -= front * velocity;
  }
  // A is moving left
  else if (keys[GLFW_KEY_A])
  {
	  position -= right * velocity;
  }
  // D is moving right
  else if (keys[GLFW_KEY_D])
  {
	  position += right * velocity;
  }
  // Reinitialise the Camera position
  else if (keys[GLFW_KEY_R])
  {
	  position = glm::vec3(0.0f, 0.0f, 0.0f);
  }
}

glm::mat4 Camera::calculateViewMatrix()
{
	// eye : where we're look'n at from, second parameter: which target we are looking at which is front in this case, third is just up
	return glm::lookAt(position, position + front, up); 
}

void Camera::mouseControl
(
	GLfloat xChange, GLfloat yChange
)
{
	xChange *= turnSpeed; // 10 pixels moved if your turnspeed is 5 pixels then you moved 50 pixels
	yChange *= turnSpeed; // 10 pixels moved if your turnspeed is 5 pixels then you moved 50 pixels

	yaw = yaw + xChange;
	pitch = pitch + yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}


Camera::~Camera()
{

}