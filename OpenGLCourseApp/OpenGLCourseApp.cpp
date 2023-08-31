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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

WNS::Window mainWindow; // Our simply window object
std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects
std::vector<SNS::Shader*> shaderList; // The vector list that holds shader objects
CNS::Camera camera; // Our simply camera object

TNS::Texture brickTexture; // Brick texture object
TNS::Texture dirtTexture; // Dirt texture object
TNS::Texture plainTexture; // Plain texture which has Alpha channel!!

MNS::Material ShinyMaterial; // our material objects which is shiny for this case
MNS::Material DullMaterial;  // Material objects which is dull now

LNS::Light *mainLight; // Light object
LNS::PointLight pointLight[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;

GLfloat deltaTime{0.0f}; // Delta -> Change!!! deltaTime -> change in time
GLfloat lastTime{ 0.0f };

// Vertex shaders
static const char* vShader = "Shaders/shader.vert.txt";
// Fragment Shader
static const char* fShader = "Shaders/shader.frag.txt";

void CalcAvgNormals
(
	unsigned int* indices, unsigned int numberOfindices, GLfloat* vertices,
	unsigned int numberOfvertices,unsigned int vLength,unsigned int normalOffset
)
{
	for (unsigned int i{ 0 }; i < numberOfindices; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 N = glm::cross(v1, v2);
		// Normalize our normal vector!
		N = glm::normalize(N);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += N.x; vertices[in0 + 1] += N.y; vertices[in0 + 2] += N.z;
		vertices[in1] += N.x; vertices[in1 + 1] += N.y; vertices[in1 + 2] += N.z;
		vertices[in2] += N.x; vertices[in2 + 1] += N.y; vertices[in2 + 2] += N.z;
	}

	for (unsigned int i{ 0 }; i < numberOfvertices / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}

}

void CreateObjects()
{  
	const int numberOfindices{ 12 }, numberOfvertices{ 32 };
	const int numberOfFloorVertices{ 32 },numberOfFloorIndices{ 6 };

    struct Indices
	{
		unsigned int indices[numberOfindices]
		{
		  0, 1, 2, // Front
	      3, 2, 1, // Right
	      3, 0, 2, // Left
	      3, 1, 0 // Bottom
		};

		unsigned int FloorIndices[numberOfFloorIndices]
		{
			0, 2, 1,
			1, 2, 3
		};

	};

    struct Vertices
	{
		// Last two points ara u and v for texture coordinates
		GLfloat vertices[numberOfvertices]  
		{
		//    X     Y    Z             U    V            nx   ny   nz
			-1.0f,-1.0f,-0.6f,        0.0f, 0.0f,         0.0f,0.0f,0.0f,
			0.0f,-1.0f,1.0f,         0.5f, 0.0f,         0.0f,0.0f,0.0f,
			1.0f,-1.0f,-0.6f,        1.0f, 0.0f,         0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,          0.5f, 1.0f,         0.0f,0.0f,0.0f
		};

		GLfloat FloorVertices[numberOfFloorVertices]
		{
			-10.0f,0.0f,-10.0f,       0.0f,0.0f,     0.0f,-1.0f,0.0f,
			 10.0f,0.0f,-10.0f,      10.0f,0.0f,     0.0f,-1.0f,0.0f,
			-10.0f,0.0f,10.0f,       0.0f ,10.0f,    0.0f,-1.0f,0.0f,
			 10.0f,0.0f,10.0f,       10.0f,10.0f,    0.0f,-1.0f,0.0f
		};

	};

	// Vertices and Indices struct objects
	Indices* Ind = new Indices;
	Vertices* Vert = new Vertices;

	CalcAvgNormals(Ind->indices, numberOfindices, Vert->vertices, numberOfvertices, 8, 5);

	// First Mesh obj
	MNS::Mesh* mesh1 = new MNS::Mesh();
	mesh1->CreateMesh(Vert->vertices,Ind->indices,numberOfvertices,numberOfindices);
	meshList.push_back(mesh1);

	// Second Mesh obj
	MNS::Mesh* mesh2 = new MNS::Mesh();
	mesh2->CreateMesh(Vert->vertices, Ind->indices, numberOfvertices, numberOfindices);
	meshList.push_back(mesh2);

	// Third Mesh Obj
	MNS::Mesh* mesh3 = new MNS::Mesh();
	mesh3->CreateMesh(Vert->FloorVertices, Ind->FloorIndices, numberOfFloorVertices, numberOfFloorIndices);
	meshList.push_back(mesh3);
}

void CreateShaders()
{
	SNS::Shader* shader1 = new SNS::Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

void CreateInstances()
{
	mainWindow = WNS::Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	camera = CNS::Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.7f);

	brickTexture = TNS::Texture("Textures/brick.png");
	brickTexture.loadTexture();
	dirtTexture = TNS::Texture("Textures/dirt.png");
	dirtTexture.loadTexture();
	plainTexture = TNS::Texture("Textures/plain.png");
	plainTexture.loadTexture();

	ShinyMaterial = MNS::Material(4.0f, 256);
	DullMaterial = MNS::Material(0.3f, 4);

	mainLight = new LNS::DirectionalLight(1.0f, 1.0f, 1.0f, 
		                                 0.0f, 0.0f,
		                                 0.0f, 0.0f, -1.0f);
	
	pointLight[0] = LNS::PointLight(0.0f, 0.0f, 1.0f,
		                            0.0f, 1.0f,
		                            0.0f, 0.0f, 0.0f,
		                            0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLight[1] = LNS::PointLight(0.0f, 1.0f, 0.0f,
		                            0.0f, 1.0f,
		                           -4.0f, 2.0f, 0.0f,
		                           0.3f, 0.1f, 0.1f);
   pointLightCount++;
}

int main()
{
	CreateInstances();
	GLuint uniformModel{ 0 }, uniformProjection{ 0 }, uniformView{ 0 };
	GLuint uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{0};

	// Create the perspective projection outside the main loop
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.GetbufferWidth() / (GLfloat)mainWindow.GetbufferHeight(), 0.1f, 150.0f);

	// Loop until window closed.
	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		mainWindow.FPSCounter();
		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXchange(), mainWindow.getYChange());

		// Clear Window

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();

		shaderList[0]->SetDirectionalLight(mainLight);
		shaderList[0]->SetPointLights(pointLight, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, 
			camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		glm::mat4 model(1.0f);   	
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));  //-> curSize * x,curSize * y, 1 * z
		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
		brickTexture.useTexture();
		ShinyMaterial.useMaterial(uniformSpecularIntensity,uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.useTexture();
		DullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.useTexture();
		ShinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	delete mainLight;

	return 0;
}
