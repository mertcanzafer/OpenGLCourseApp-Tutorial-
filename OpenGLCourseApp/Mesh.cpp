#include "Mesh.h"
using namespace MNS;

Mesh::Mesh()
	:
	VAO{0},VBO{0},IBO{0},indexCount{0}{}

// Implement the functions

void Mesh::CreateMesh
(
	GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices
)

{
	this->indexCount = numOfIndices;

	// Creating VAOs
	glGenVertexArrays(1, &VAO); // Where do you store your IDs
	// Binding
	glBindVertexArray(VAO);

	// buffer for IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*vertices) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(*vertices) * 5, reinterpret_cast<void*>(sizeof(*vertices) * 3));
	glEnableVertexAttribArray(1);
	// Unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// You should unbind IBO AFTER you unbinding the VAO!!!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		// Just checking possible errors
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		// Just checking possible errors
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		// Just checking possible errors
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indexCount = 0;

}

Mesh::~Mesh() 
{
	ClearMesh();
}