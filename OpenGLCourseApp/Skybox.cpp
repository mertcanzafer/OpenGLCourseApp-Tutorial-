#include "Skybox.h"
using namespace SKNS;
Skybox::Skybox()
{
}

Skybox::Skybox(std::vector<std::string> faceLocations)
	:skyShader{nullptr},skyMesh{nullptr}
{
	// Shader setup
	skyShader = new SNS::Shader();
	skyShader->CreateFromFiles("Shaders/skybox.vert.txt","Shaders/skybox.frag.txt");

	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();	

    //	Texture setup
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, bitDepth;

	for (size_t i = 0; i < NUM_OF_FACES; i++)
	{
		unsigned char* textData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);

		// If there is no file location
		if (!textData) { std::cout << "Failed to find: " << faceLocations[i] << std::endl;  return; }

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textData);
		// Deallocates the memory 
		stbi_image_free(textData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh setup
	
	unsigned int skyboxIndices[]
	{
		    // Front
		    0, 1, 2,
			2, 1, 3,
			//right
			2, 3, 5,
			5, 3, 7,
			// back
			5, 7, 4,
			4, 7, 6,
			// left
			4, 6, 0,
			0, 6, 1,
			// top
			4, 0, 5,
			5, 0, 2,
			// bottom
			1, 6, 3,
			3, 6, 7
	};

	float skyboxVertices[]
	{
	   -1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	skyMesh = new MNS::Mesh();
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

Skybox::Skybox(const Skybox& copy)
{
	// Shallow copy part
	this->textureId = copy.textureId;
	this->uniformProjection = copy.uniformProjection;
	this->uniformView = copy.uniformView;

	// Deep copy part
	skyShader = new SNS::Shader();
	skyShader = copy.skyShader;

	skyMesh = new MNS::Mesh();
	skyMesh = copy.skyMesh;
}

void Skybox::operator=(const Skybox& copy)
{
	// Shallow copy part
	this->textureId = copy.textureId;
	this->uniformProjection = copy.uniformProjection;
	this->uniformView = copy.uniformView;

	// Deep copy part
	skyShader = new SNS::Shader();
	skyShader = copy.skyShader;

	skyMesh = new MNS::Mesh();
	skyMesh = copy.skyMesh;
}

void Skybox::DrawSkybox
(
	glm::mat4 viewMatrix, glm::mat4 projectionMatrix
)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);

	skyShader->UseShader();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	skyShader->Validate();

	skyMesh->RenderMesh();

	glDepthMask(GL_TRUE);
}
