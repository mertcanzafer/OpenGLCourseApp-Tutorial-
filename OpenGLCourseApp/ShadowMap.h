#pragma once

#include <iostream>
#include <GL\glew.h>

namespace SMN 
{
	class ShadowMap
	{
	  public:

		 ShadowMap();
		 // Virtual functions for overriding from base class to derived classes
		 virtual bool Init(GLuint width, GLuint height);
		 virtual void Write();
		 virtual void Read(GLenum textureUnit);
		 
		 //Getters
		 GLuint GetShadowWidth() { return shadowWidth; }
		 GLuint GetShadowHeight() { return  shadowHeight; }

		 void Free();
		 virtual ~ShadowMap();

	  protected:
		  GLuint FBO, shadowMap; // shadowMap will contain id of shadow map texture
		  GLuint shadowWidth, shadowHeight;
	};	
}
