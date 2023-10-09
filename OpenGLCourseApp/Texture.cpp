#include "Texture.h"
using namespace TNS;

int Texture::TextureObjectCount = 0;

Texture::Texture()
	:textureID{0},width{0},height{0},bitDepth{0},fileLocation{nullptr}{}

Texture::Texture(const char* fileLoc) :Texture()
{
	TextureObjectCount++;
	fileLocation = fileLoc;
} 

bool Texture::loadTexture()
{
    unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth,0);

	// If there is no file location
	if (!textData) { std::cout << "Failed to find: " << fileLocation << std::endl;  return false; }

	// Binding and Unbinding out texture just like we did back in VBO,VAO,IBO...
	glGenTextures(1, &textureID);
	// GL_TEXTURE_3Ds are used for things such as volumetric data.
	// In modern shooter games like battlefield games, If you're flying a plane through
	// a cloud, there should be textures on the inside as the cloud is really a more advanced form of texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// If you have an issue with the texture itself just change RGBA <-> RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textData);

	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// Deallocates the memory 
	stbi_image_free(textData);
	return true;
}

bool Texture::loadTextureA()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	// If there is no file location
	if (!textData) { std::cout << "Failed to find: " << fileLocation << std::endl;  return false; }

	// Binding and Unbinding out texture just like we did back in VBO,VAO,IBO...
	glGenTextures(1, &textureID);
	// GL_TEXTURE_3Ds are used for things such as volumetric data.
	// In modern shooter games like battlefield games, If you're flying a plane through
	// a cloud, there should be textures on the inside as the cloud is really a more advanced form of texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// If you have an issue with the texture itself just change RGBA <-> RGB
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);

	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	// Deallocates the memory 
	stbi_image_free(textData);
	return true;
}

void Texture::useTexture()
{
	glActiveTexture(GL_TEXTURE1); // Texture unit 1
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = width = height = bitDepth = 0;
	fileLocation = nullptr;
}

Texture::~Texture()
{
	clearTexture();
}
