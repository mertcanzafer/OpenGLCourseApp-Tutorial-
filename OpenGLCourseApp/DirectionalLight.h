#pragma once
#include "Light.h"

namespace LNS 
{
	class DirectionalLight : public Light
	{
	  public:
		  DirectionalLight();
		  // Overloaded Constructor
		  DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
			  GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
			  GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);

		  // Function overriding with Inheritance
		  void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
			  GLfloat diffuseIntensityLoc, GLfloat directionLoc);

		  glm::mat4 CalcLightTransform()override;

		 virtual ~DirectionalLight();
	  private:
		  glm::vec3 direction; //The direction of our light
	};
}

