#pragma once

#include<cstdio>
#include <iostream>
#include<cstring>
#include<fstream>
#include <string>

#include <Gl\glew.h>


namespace SNS 
{
	class Shader
	{
	  public:
		 Shader();

		 void CreateFromString(const char* vertexCode,const char* fragmentCode);
		 void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

		 std::string ReadFile(const char* filePath);
		 // Getters
		 GLuint GetProjectionLocation();
		 GLuint GetModelLocation();
		 GLuint GetViewLocation();
		 GLuint GetAmbientIntensityLocation();
		 GLuint GetAmbientColourLocation();
		 GLuint GetDiffuseIntensityLocation();
		 GLuint GetDirectionLocation();

		 void UseShader();
		 void ClearShader();

		 ~Shader();

	  private:

		  /*
		    UniformModel: An uniform value how much moved coordinates along the x axis.
			UniformProjection: The variable that holds id and location for projection in vertex shader.
			uniformView: for the camera
		  */

		 GLuint shaderID, uniformModel, uniformProjection,uniformView; 
		 GLuint uniformAmbientIntensity, uniformAmbientColour;
		 GLuint uniformDiffuseIntensity, uniformDirection;

		 void CompileShader(const char* vertexCode, const char* fragmentCode);
		 void AddShader(GLuint* theProgram, const char* shaderCode, GLenum shaderType);

	};
}

