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
		  Light(GLfloat red,GLfloat green,GLfloat blue,GLfloat aIntensity,GLfloat dIntensity);

		  virtual void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
			  GLfloat diffuseIntensityLoc, GLfloat directionLoc) = 0;

		 virtual ~Light();
	
	  protected:
		  // First variable is color of our light
		  glm::vec3 colour;
		  GLfloat ambientIntensity; // Intensity of our light source.
		  GLfloat diffuseIntensity;
	};
}


