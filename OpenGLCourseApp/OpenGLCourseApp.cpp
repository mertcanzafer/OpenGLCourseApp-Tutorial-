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

const GLint width = 800, height = 600;
const float toRadians = 3.14159265f / 180.0f;

std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects

GLuint shader;
GLuint uniformModel;  // An uniform value how much moved coordinates along the x axis.
GLuint uniformProjection; // The variable that holds id and location for projection in vertex shader.

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

static const char* vShader = "                                \n\
#version 330                                                  \n\
                                                              \n\
uniform mat4 model;                                           \n\
uniform mat4 projection;                                      \n\
                                                              \n\
layout (location = 0) in vec3 pos;                            \n\
                                                              \n\
out vec4 vColor;	                                          \n\
	                                                          \n\
void main()                                                   \n\
{                                                             \n\
	gl_Position = projection * model * vec4(pos, 1.0);        \n\
    vColor = vec4(clamp(pos,0.0f,1.0f),1.0f);                 \n\
}";

// Fragment Shader

static const char* fShader = "                                \n\
#version 330                                                  \n\
                                                              \n\
in vec4 vColor;                                               \n\
                                                              \n\
out vec4 color;                                               \n\
                                                              \n\
void main()                                                   \n\
{                                                             \n\
	color = vColor;                                           \n\
}";

void CreateTriangle()
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

// Implement Shaders or Apply them

void AddShader(GLuint *theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1]{};
	theCode[0] = shaderCode;

	GLint codeLength[1]{};
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);

	glCompileShader(theShader);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

 	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
		std::cout << "Error compiling the " << shaderType << " shader: " << elog << std::endl;
		return;
	}
	glAttachShader(*theProgram, theShader);
}


void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		std::cout << "Error creating shader program" << std::endl;
		return;
	}
	AddShader(&shader, vShader, GL_VERTEX_SHADER);
	AddShader(&shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		std::cout << "Error linking program  " << elog << std::endl;
		return;
	}
	// Validating our program. Remember That's really important in OpenGL Apps
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		std::cout << "Error validating program  " << elog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
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

	CreateTriangle();
	CompileShaders();

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

		glUseProgram(shader);

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
