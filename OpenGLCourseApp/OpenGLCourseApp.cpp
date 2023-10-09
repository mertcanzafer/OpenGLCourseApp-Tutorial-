#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cstring>
#include <cmath>
#include<vector>
#include <windows.h>

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
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

// This program is forced to run on NVIDIA Graphics Card!!!!!!!
extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

const float toRadian{ 0.0174532925 };

WNS::Window mainWindow; // Our simply window object
std::vector<MNS::Mesh*> meshList; // The vector list that holds mesh objects
std::vector<SNS::Shader*> shaderList; // The vector list that holds shader objects
SNS::Shader directionalShadowShader; // Separate ShadowMapShader object
SNS::Shader omniShadowShader;	// Omni ShadowMap shader 
CNS::Camera camera; // Our simply camera object

TNS::Texture brickTexture; // Brick texture object
TNS::Texture dirtTexture; // Dirt texture object
TNS::Texture plainTexture; // Plain texture which has Alpha channel!!

MNS::Material ShinyMaterial; // our material objects which is shiny for this case
MNS::Material DullMaterial;  // Material objects which is dull now

MNS::Model xWing; // Starwars aircraft model
MNS::Model BlackHawk;// Armed helicopter

LNS::Light *mainLight; // Light object
LNS::PointLight pointLight[MAX_POINT_LIGHTS];
LNS::SpotLight spotLight[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime{0.0f}; // Delta -> Change!!! deltaTime -> change in time
GLfloat lastTime{ 0.0f };

GLfloat blaclHawkAngle{ 0.0f };

GLuint uniformModel{ 0 }, uniformProjection{ 0 }, uniformView{ 0 };
GLuint uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{ 0 };
GLuint uniformOmniLightPos{ 0 }, uniformFarPlane{ 0 };

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
			-15.0f,0.0f,-15.0f,       0.0f,0.0f,     0.0f,-1.0f,0.0f,
			 15.0f,0.0f,-15.0f,      15.0f,0.0f,     0.0f,-1.0f,0.0f,
			-15.0f,0.0f,15.0f,       0.0f ,15.0f,    0.0f,-1.0f,0.0f,
			 15.0f,0.0f,15.0f,       15.0f,15.0f,    0.0f,-1.0f,0.0f
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

	delete Ind; delete Vert;
	mesh1 = nullptr; mesh2 = nullptr; mesh3 = nullptr;
} 

void CreateShaders()
{
	SNS::Shader* shader1 = new SNS::Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);

	directionalShadowShader = SNS::Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert.txt",
		"Shaders/directional_shadow_map.frag.txt");

	omniShadowShader = SNS::Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert.txt", "Shaders/omni_shadow_map.frag.txt",
		"Shaders/omni_shadow_map.geom.txt");
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

	xWing = MNS::Model();
	xWing.LoadModel("Models/x-wing.obj");
	BlackHawk = MNS::Model();
	BlackHawk.LoadModel("Models/uh60.obj");
		
	mainLight = new LNS::DirectionalLight(2048.0f,2048.0f,
		                                 1.0f, 1.0f, 1.0f, 
		                                 0.0f, 0.1f,
		                                 0.0f, -15.0f, -10.0f);
	
	pointLight[0] = LNS::PointLight( 1024,1024,
		                            0.01f,100.0f,
		                            0.0f, 0.0f, 1.0f,
		                            0.0f, 1.0f,
		                            1.0f, 3.0f, 0.0f,
		                            0.3f, 0.1f, 0.1f);
	pointLightCount++;

	pointLight[1] = LNS::PointLight(1024, 1024,
		                            0.01f, 100.0f,
		                            0.0f, 1.0f, 0.0f,
		                            0.0f, 0.0f,
		                           -4.0f, 3.0f, 0.0f,
		                           0.3f, 0.1f, 0.1f);
   pointLightCount++;

   spotLight[0] = LNS::SpotLight(1024, 1024,
							     0.01f, 100.0f,
	                             1.0f, 1.0f, 1.0f,
	                             0.0f, 1.0f,
	                             0.0f, 0.0f, 0.0f,
	                             0.0f, -1.0f, 0.0f,
	                             1.0f, 0.0f, 0.0f,
	                             20.0f);
   spotLightCount++;

   spotLight[1] = LNS::SpotLight(1024, 1024,
								 0.01f, 100.0f,
							     1.0f, 1.0f, 1.0f,
	                             0.0f, 2.0f,
	                             0.0f, -0.5f, 0.0f,
	                            -2.5f, -1.0f, 0.0f,
	                             1.0f, 0.0f, 0.0f,
	                             15.0f);
   spotLightCount++;
}

void RenderScene()
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));  //-> curSize * x,curSize * y, 1 * z
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.useTexture();
	ShinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
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
	dirtTexture.useTexture();
	ShinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-6.5f, 0.5f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ShinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	xWing.RenderModel();

	blaclHawkAngle += 0.06f; //CCW -> CW = - -> +
	if (blaclHawkAngle > 360.0f)
	{
		blaclHawkAngle = 0.06f;
	}
	model = glm::mat4(1.0f);
	model = glm::rotate(model, -blaclHawkAngle * toRadian, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotated around y axis
	model = glm::translate(model, glm::vec3(-8.0f, 2.0f, 4.0f));
	model = glm::rotate(model, -20.0f * toRadian, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, -90.0f * toRadian, glm::vec3(1.0f, 0.0f, 0.0f)); //Rotated around x axis
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	ShinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
	BlackHawk.RenderModel();
}

void DirectionalShadowMapPass(LNS::Light* mlight)
{

	directionalShadowShader.UseShader();

	glViewport(0, 0, mlight->GetShadowMap()->GetShadowWidth(), mlight->GetShadowMap()->GetShadowHeight());

	mlight->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	
	glm::mat4 ligthTransform = mlight->CalcLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&ligthTransform);

	directionalShadowShader.Validate();
	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(LNS::PointLight* plight)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, plight->GetShadowMap()->GetShadowWidth(), plight->GetShadowMap()->GetShadowHeight());

	plight->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPos();
	uniformFarPlane = omniShadowShader.GetFarPlaneLoc();

	glUniform3f(uniformOmniLightPos, plight->GetPosition().x, plight->GetPosition().y, plight->GetPosition().z);
	glUniform1f(uniformFarPlane, plight->GetFarPlane()); 

	omniShadowShader.setLightMatrices(plight->calcLightTransform());

	omniShadowShader.Validate();	

	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4* projectionMatrix,glm::mat4 viewMatrix)
{
	shaderList[0]->UseShader();

	uniformModel = shaderList[0]->GetModelLocation();
	uniformProjection = shaderList[0]->GetProjectionLocation();
	uniformView = shaderList[0]->GetViewLocation();
	uniformEyePosition = shaderList[0]->GetEyePositionLocation();
	uniformShininess = shaderList[0]->GetShininessLocation();
	uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();

	glViewport(0, 0, 1366, 768);
	
	// Clear Window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x,
		camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaderList[0]->SetDirectionalLight(mainLight);
	shaderList[0]->SetPointLights(pointLight, pointLightCount,0,3);
	shaderList[0]->SetSpotLights(spotLight, spotLightCount,pointLightCount,3 + pointLightCount);
	glm::mat4 ligthTransform = mainLight->CalcLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&ligthTransform);

	mainLight->GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0]->SetTexture(1);
	shaderList[0]->SetDirectionalShadowMap(2);

	glm::vec3 LowerLight = camera.GetCameraPosition();
	LowerLight.y -= 0.5f;
	spotLight[1].SetFlash(LowerLight, camera.GetCameraDireciton());

	shaderList[0]->Validate();

	RenderScene();
}

int main()
{
	CreateInstances();
	// Create the perspective projection outside the main loop
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.GetbufferWidth() / (GLfloat)mainWindow.GetbufferHeight(), 0.1f, 150.0f);

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

		if (mainWindow.getKeys()[GLFW_KEY_L])
		{
			spotLight[1].Toggle();
			mainWindow.getKeys()[GLFW_KEY_L] = false;
		}

		DirectionalShadowMapPass(mainLight);
		
		for (size_t i{ 0 }; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLight[i]);
		}
		for (size_t i{ 0 }; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLight[i]);
		}

		RenderPass(&projection,camera.calculateViewMatrix());

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	for (auto& m : meshList)
	{
		delete m;
	}

	for (auto& s : shaderList)
	{
		delete s;
	}

	meshList.clear();
	shaderList.clear();

	delete mainLight;

	return 0;
}
