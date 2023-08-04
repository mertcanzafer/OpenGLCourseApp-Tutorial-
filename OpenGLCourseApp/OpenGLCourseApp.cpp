#include <iostream>
#include <cstring>
#include <cmath>
#include<vector>

#include <Gl\glew.h>
#include<GLFW\glfw3.h>

#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

const GLint width = 800, height = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects
std::vector<SNS::Shader*> shaderList; // The vector list that holds shader objects

bool Direction = true;
float tryOffset = 0.0f;
float tryMaxOffset = 0.7f;
float tryIncrement = 0.0012f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
float sizeIncrement = 0.001f;

// Vertex shaders

static const char* vShader = "Shaders/shader.vert.txt";

// Fragment Shader

static const char* fShader = "Shaders/shader.frag.txt";

void CreateObjects()
{  
	const int numberOfindices{ 12 }, numberOfvertices{ 12 };
	unsigned int indices[numberOfindices]
	{
		0,1,3,
        1,3,2,
		2,3,0,
	    0,1,2
	};

	GLfloat vertices[numberOfvertices]
	{
		-1.0f,-1.0f,0.0f,
		0.0f,-1.0f,1.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};

	// First Mesh obj
	MNS::Mesh* mesh1 = new MNS::Mesh();
	mesh1->CreateMesh(vertices,indices,numberOfvertices,numberOfindices);
	meshList.push_back(mesh1);

	// Second Mesh obj
	MNS::Mesh* mesh2 = new MNS::Mesh();
	mesh2->CreateMesh(vertices, indices, numberOfvertices, numberOfindices);
	meshList.push_back(mesh2);

}

void CreateShaders()
{
	SNS::Shader* shader1 = new SNS::Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}


int main()
{

	if (!glfwInit())
	{
		std::cout << "GLFW initialisation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}
	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set the profile
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);

	// Creating our window...

	GLFWwindow* MainWindow = glfwCreateWindow(width,height,"3D Game Engine",NULL,NULL);
	if (!MainWindow)
	{
		std::cout << "GLFW window creation failed!"<<std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size Info.
	int buffersizeHeight,buffersizeWidth;
	glfwGetFramebufferSize(MainWindow, &buffersizeWidth, &buffersizeHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(MainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation failed!"<<std::endl;
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, buffersizeWidth, buffersizeHeight);

	CreateObjects();
	CreateShaders();

	GLuint uniformModel{ 0 }, uniformProjection{ 0 };
	// Create the perspective projection outside the main loop
	glm::mat4 projection = glm::perspective(45.0f, static_cast<GLfloat>(buffersizeWidth) / static_cast<GLfloat>(buffersizeHeight), 0.1f, 100.0f);


	// Loop until window closed.

	while (!glfwWindowShouldClose(MainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		if (Direction)
		{
			tryOffset += tryIncrement;
		}
		else {
			tryOffset -= tryIncrement;
		}

		if (abs(tryOffset) >= tryMaxOffset) {
			
			Direction = !Direction;
		}

		curAngle += 0.045f;

		if (curAngle >= 360.0f) {
			curAngle -= 360.0f;
		}
		
		if (sizeDirection) {
			curSize += sizeIncrement;
		}
		else {
			curSize -= sizeIncrement;
		}

		if (curSize >= maxSize || curSize <= minSize)
			sizeDirection = !sizeDirection;

		// Clear Window

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model(1.0f);   	
		model = glm::translate(model, glm::vec3(tryOffset, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));  //-> curSize * x,curSize * y, 1 * z
		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-tryOffset, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		meshList[1]->RenderMesh();

		glUseProgram(0);
		glfwSwapBuffers(MainWindow);
	}

	return 0;
}