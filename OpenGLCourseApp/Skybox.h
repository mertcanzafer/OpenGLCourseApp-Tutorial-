#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Shader.h"
#include "Mesh.h"

namespace SKNS
{
	class Skybox
	{
     	public:
			Skybox();

			Skybox(std::vector<std::string> faceLocations);

			Skybox(const Skybox& copy);
			void operator=(const Skybox& copy);

			void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

			~Skybox() = default;
	    private:
			MNS::Mesh* skyMesh;
			SNS::Shader* skyShader;

			GLuint textureId,uniformProjection,uniformView;
	};
}
