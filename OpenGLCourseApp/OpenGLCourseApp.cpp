#define STB_IMAGE_IMPLEMENTATION

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
#include "Camera.h"
#include "Texture.h"

const float toRadians = 3.14159265f / 180.0f;

WNS::Window mainWindow; // Our simply window object
std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects
std::vector<SNS::Shader*> shaderList; // The vector list that holds shader objects
CNS::Camera camera; // Our simply camera object

TNS::Texture brickTexture; // Brick texture object
TNS::Texture dirtTexture; // Dirt texture object

GLfloat deltaTime{0.0f}; // Delta -> Change!!! deltaTime -> change in time
GLfloat lastTime{ 0.0f };

// Vertex shaders
static const char* vShader = "Shaders/shader.vert.txt";
// Fragment Shader
static const char* fShader = "Shaders/shader.frag.txt";

void CreateObjects()
{  
	const int numberOfindices{ 12 }, numberOfvertices{ 20 };

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
		GLfloat vertices[numberOfvertices]  // Last two points ara u and v for texture coordinates
		{
		//    X     Y    Z             U    V 
			-1.0f,-1.0f,0.0f,        0.0f, 0.0f,
			0.0f,-1.0f,1.0f,         0.5f, 0.0f,
			1.0f,-1.0f,0.0f,         1.0f, 0.0f,
			0.0f,1.0f,0.0f,          0.5f, 1.0f
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
	mainWindow = WNS::Window(1024,720);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	camera = CNS::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.7f);
	
	brickTexture = TNS::Texture("Textures/brick.png");
	brickTexture.loadTexture();
	dirtTexture = TNS::Texture("Textures/dirt.png");
	dirtTexture.loadTexture();
	
	GLuint uniformModel{ 0 }, uniformProjection{ 0 }, uniformView{0};
	// Create the perspective projection outside the main loop
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.GetbufferWidth() / (GLfloat)mainWindow.GetbufferHeight(), 0.1f, 100.0f);

	// Loop until window closed.

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(),deltaTime);
		camera.mouseControl(mainWindow.getXchange(), mainWindow.getYChange());

		// Clear Window

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		glm::mat4 model(1.0f);   	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));  //-> curSize * x,curSize * y, 1 * z
		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.useTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
