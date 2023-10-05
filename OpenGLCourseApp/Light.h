#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>

#include <vector>

#include "ShadowMap.h"

namespace LNS
{
	class Light
	{
	  public:
		  Light();
		  // Overloaded Constructor
		  Light(GLfloat shadowWidth,GLfloat shadowHeight,
			  GLfloat red,GLfloat green,GLfloat blue,
			  GLfloat aIntensity,GLfloat dIntensity);

		  virtual void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
			  GLfloat diffuseIntensityLoc, GLfloat directionLoc) = 0;
		  virtual glm::mat4 CalcLightTransform() = 0;

		  Light(const Light& copy);
		  void operator=(const Light& copy);

		  SMN::ShadowMap* GetShadowMap() { return shadowMap; }

		 virtual ~Light();
	
	  protected:
		  // First variable is color of our light
		  glm::vec3 colour;
		  GLfloat ambientIntensity; // Intensity of our light source.
		  GLfloat diffuseIntensity;

		  SMN::ShadowMap* shadowMap;
		  glm::mat4 LightProj;

	};
}


