#pragma once
#include <iostream>

#include <GL\glew.h>

#include "CommonValues.h"

namespace TNS
{
	class Texture
	{
	  public:
          
		  static int TextureObjectCount;

		  Texture();
		  Texture(const char* fileLoc);

		  bool loadTexture();
		  bool loadTextureA();

		  void useTexture();
		  void clearTexture();

		  ~Texture();

	  private:
		  GLuint textureID;
		  int width, height, bitDepth; // For the image we load in for the texture

		  const char* fileLocation;

	};

}

