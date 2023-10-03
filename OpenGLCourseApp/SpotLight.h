#pragma once
#include "PointLight.h"

namespace LNS
{
	class SpotLight : public PointLight
	{
	   public:
		   SpotLight();
		   // Overloaded Ctor
		   SpotLight(GLfloat red, GLfloat green, GLfloat blue,
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat xDir,GLfloat yDir, GLfloat zDir,
			   GLfloat Constant, GLfloat Linear, GLfloat Exponent,
			   GLfloat Edge);

		   void useLight(GLuint ambientIntensityLoc, GLuint ambientColorLoc,
			   GLuint diffuseIntensityLoc, GLuint positionLoc, GLuint directionLoc,
			   GLuint constantLoc, GLuint linearLoc, GLuint exponentLoc,
			   GLuint edgeLoc);

		   void UseLight(GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
			   GLfloat diffuseIntensityLoc, GLfloat directionLoc) {}

		   void SetFlash(glm::vec3 pos, glm::vec3 dir);

		   glm::mat4 CalcLightTransform()override { return LightProj; }

		  ~SpotLight();
	   private:
		   // This is the direction the spotlight is actually facing in.
		   glm::vec3 direction;

		   GLfloat edge,procEdge;
	};
}
