#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

namespace LNS
{
	class Light
	{
	  public:
		  Light();
		  // Overloaded Constructor
		  Light(GLfloat red,GLfloat green,GLfloat blue,GLfloat aIntensity);

		  void UseLight(GLfloat ambientIntensityLoc,GLfloat ambientColorLoc);

		  ~Light();
	
	  private:
		  // First variable is color of our ligth
		  glm::vec3 colour;
		  GLfloat ambientIntensity; // Intensity of our light source.

	};
}


