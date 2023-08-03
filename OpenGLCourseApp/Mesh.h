#pragma once

#include <Gl\glew.h>

namespace MNS 
{
	class Mesh
	{
	public:
		Mesh();

		void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
		void RenderMesh();
		void ClearMesh();

		~Mesh();

	private:

		GLuint VAO, VBO, IBO; // IBO: Index buffer object.
		GLsizei indexCount;

	};
}



