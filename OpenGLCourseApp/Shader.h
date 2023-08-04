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

		 GLuint GetProjectionLocation();
		 GLuint GetModelLocation();

		 void UseShader();
		 void ClearShader();

		 ~Shader();

	  private:

		  /*
		    UniformModel: An uniform value how much moved coordinates along the x axis.
			UniformProjection: The variable that holds id and location for projection in vertex shader.
		  */

		 GLuint shaderID, uniformModel, uniformProjection; 

		 void CompileShader(const char* vertexCode, const char* fragmentCode);
		 void AddShader(GLuint* theProgram, const char* shaderCode, GLenum shaderType);

	};
}

