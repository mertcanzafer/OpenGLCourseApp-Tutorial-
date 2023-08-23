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
		  Light(GLfloat red,GLfloat green,GLfloat blue,GLfloat aIntensity,
			  GLfloat xDir,GLfloat yDir,GLfloat zDir,GLfloat dIntensity);

		  void UseLight(GLfloat ambientIntensityLoc,GLfloat ambientColorLoc,
			  GLfloat diffuseIntensityLoc,GLfloat directionLoc);

		  ~Light();
	
	  private:
		  // First variable is color of our light
		  glm::vec3 colour;
		  GLfloat ambientIntensity; // Intensity of our light source.

		  glm::vec3 direction; //The direction of our light
		  GLfloat diffuseIntensity;
	};
}


