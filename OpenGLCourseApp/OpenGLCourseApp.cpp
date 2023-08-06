#include <iostream>
#include <cstring>
#include <cmath>
#include<vector>

#include <Gl\glew.h>
#include<GLFW\glfw3.h>

#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

const float toRadians = 3.14159265f / 180.0f;

WNS::Window mainWindow; // Our simply window object
std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects
std::vector<SNS::Shader*> shaderList; // The vector list that holds shader objects

// Vertex shaders
static const char* vShader = "Shaders/shader.vert.txt";
// Fragment Shader
static const char* fShader = "Shaders/shader.frag.txt";

void CreateObjects()
{  
	const int numberOfindices{ 12 }, numberOfvertices{ 12 };

    struct Indices
	{
		unsigned int indices[numberOfindices]
		{
			0,1,3,
			1,3,2,
			2,3,0,
			0,1,2
		};
	};

    struct Vertices
	{
		GLfloat vertices[numberOfvertices]
		{
			-1.0f,-1.0f,0.0f,
			0.0f,-1.0f,1.0f,
			1.0f,-1.0f,0.0f,
			0.0f,1.0f,0.0f
		};
	};

	// Vertices and Indices struct objects
	Indices* Ind = new Indices;
	Vertices* Vert = new Vertices;

	// First Mesh obj
	MNS::Mesh* mesh1 = new MNS::Mesh();
	mesh1->CreateMesh(Vert->vertices,Ind->indices,numberOfvertices,numberOfindices);
	meshList.push_back(mesh1);

	// Second Mesh obj
	MNS::Mesh* mesh2 = new MNS::Mesh();
	mesh2->CreateMesh(Vert->vertices, Ind->indices, numberOfvertices, numberOfindices);
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
	mainWindow = WNS::Window(800,600);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	GLuint uniformModel{ 0 }, uniformProjection{ 0 };
	// Create the perspective projection outside the main loop
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetbufferWidth() / mainWindow.GetbufferHeight(), 0.1f, 100.0f);

	// Loop until window closed.

	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Clear Window

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model(1.0f);   	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));  //-> curSize * x,curSize * y, 1 * z
		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
