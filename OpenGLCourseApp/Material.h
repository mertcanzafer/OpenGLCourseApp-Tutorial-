#pragma once

#include<GL\glew.h>

namespace MNS 
{
	class Material
	{
	  public:
		  Material();
		  Material(GLfloat sIntensity, GLfloat shine);

		  void useMaterial(GLuint specularIntensityLoc,GLuint shininessLoc);

		  ~Material();
	  private:
		  // Specular Intensiry is the intensity that how much specular light there should be on the object.
		  GLfloat specularIntensity;
		  GLfloat shininess;

	};

}
