#pragma once

#include<cstdio>
#include <iostream>
#include<cstring>
#include<fstream>
#include <string>

#include <Gl\glew.h>
#include<glm\glm.hpp>
#include<glm\gtc\type_ptr.hpp>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"

namespace SNS 
{
	class Shader
	{
	  public:
		 Shader();

		 void CreateFromString(const char* vertexCode,const char* fragmentCode);
		 void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
		 // Overload the create from files function to include geomShader
		 void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation,const char* geometryLocation);

		 std::string ReadFile(const char* filePath);
		 // Getters
		inline GLuint GetModelLocation() const { return uniformModel; }
		inline GLuint GetProjectionLocation()const { return uniformProjection; }
		inline GLuint GetViewLocation()const { return uniformView; }
		inline GLuint GetAmbientIntensityLocation()const { return uniformDirectionalLight.uniformAmbientIntensity; }
		inline GLuint GetAmbientColourLocation()const { return uniformDirectionalLight.uniformColour; }
	    inline GLuint GetDirectionLocation()const { return uniformDirectionalLight.uniformDirection; }
	    inline GLuint GetDiffuseIntensityLocation()const { return uniformDirectionalLight.uniformDiffuseIntensity; }
	    inline GLuint GetShininessLocation()const { return uniformShininess; }
	    inline GLuint GetSpecularIntensityLocation()const { return uniformSpecularIntensity; }
	    inline GLuint GetEyePositionLocation()const { return unformEyePosition; }
	    inline GLuint GetFarPlaneLoc()const { return uniformFarPlane; }
	    inline GLuint GetOmniLightPos()const { return uniformOmniLightPos; }

		 void SetDirectionalLight(LNS::Light* mLight);
		 void SetPointLights(LNS::PointLight* pLight,unsigned int LightCount);
		 void SetSpotLights(LNS::SpotLight* sLight, unsigned int sLightCount);
		 void SetTexture(GLuint textureUnit);
		 void SetDirectionalShadowMap(GLuint textureUnit);
		 void SetDirectionalLightTransform(glm::mat4* lTransform); 
		 void setLightMatrices(std::vector<glm::mat4> lightMatrices);

		 void UseShader();
		 void ClearShader();

		 ~Shader();

	  private:

		  int pointLightCount;
		  int spotLightCount;
		  GLuint uniformPointLightCount,uniformSpotLightCount;

		  struct 
		  {
			  GLuint uniformColour;
			  GLuint uniformAmbientIntensity;
			  GLuint uniformDiffuseIntensity;

			  GLuint uniformDirection;
		  }uniformDirectionalLight;

		  struct
		  {
			  GLuint uniformColour;
			  GLuint uniformAmbientIntensity;
			  GLuint uniformDiffuseIntensity;

			  GLuint uniformPosition;
			  GLuint uniformConstant;
			  GLuint uniformLinear;
			  GLuint uniformExponent;
		  }uniformPointLight[MAX_POINT_LIGHTS];

		  struct 
		  {
			  GLuint uniformColour;
			  GLuint uniformAmbientIntensity;
			  GLuint uniformDiffuseIntensity;

			  GLuint uniformPosition;
			  GLuint uniformConstant;
			  GLuint uniformLinear;
			  GLuint uniformExponent;

			  GLuint uniformDirection;
			  GLuint uniformEdge;

		  }uniformSpotLight[MAX_SPOT_LIGHTS];

		  /*
		    UniformModel: An uniform value how much moved coordinates along the x axis.
			UniformProjection: The variable that holds id and location for projection in vertex shader.
			uniformView: for the camera
		  */

		 GLuint shaderID, uniformModel, uniformProjection,uniformView; 
		 GLuint unformEyePosition, uniformSpecularIntensity, uniformShininess;
		 GLuint uniformDirectionalLightTransform, uniformDirectionalShadowMap,uniformTexture;
		 GLuint uniformOmniLightPos, uniformFarPlane;

		 GLuint uniformLightMatrices[NUM_OF_FACES];

		 void CompileShader(const char* vertexCode, const char* fragmentCode);
		 // Overload this one as well
		 void CompileShader(const char* vertexCode, const char* fragmentCode,const char* geometryCode);
		 void AddShader(GLuint* theProgram, const char* shaderCode, GLenum shaderType);
		 void HandlePointLights();
		 void HandleSpotLights();
		 void HandleCubeMapFaces();
		 void BindUniformIDs();
	};
}

