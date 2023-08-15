#pragma once
#include <iostream>

#include <GL\glew.h>

#include "stb_image.h"

namespace TNS
{
	class Texture
	{
	  public:
          
		  Texture();
		  Texture(const char* fileLoc);

		  void loadTexture();
		  void useTexture();
		  void clearTexture();

		  ~Texture();

	  private:
		  GLuint textureID;
		  int width, height, bitDepth; // For the image we load in for the texture

		  const char* fileLocation;

	};

}

