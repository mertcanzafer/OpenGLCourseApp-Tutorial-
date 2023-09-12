#include "Model.h"
using namespace MNS;

Model::Model(){}

void Model::RenderModel()
{
	for (size_t i{ 0 }; i < meshList.size(); i++)
	{
		unsigned int materialInd = MeshToText[i];

		if (materialInd < textureList.size() && textureList[materialInd])
		{
			textureList[materialInd]->useTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel
(
	const std::string& filename
)
{
	// Calling ClearModel() here allows you to reuse the same object from the class for loading different model. 
	ClearModel();

	Assimp::Importer importer;

	/*
	    Creating our actual scene:
	    aiProcess_Triangulate: That will force assimp to split up more complex shapes into triangles.
	    aiProcess_FlipUVs: That will flip all of our texture values along the y(up and down) axis.
        aiProcess_GenSmoothNormals: This one will generate smooth normals for shapes that have no normals defined.
		aiProcess_JoinIdenticalVertices: If there are two vertices at the same point which is identical,
		that will join them in one point.
	*/

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate |
		aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
         
	if (!scene)
	{
		std::cout << "Model ("<<filename <<") failed to load : " << importer.GetErrorString()<<std::endl;
		return;
	}
	// mRoorNode: the root node of the hierarchy.
	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::LoadNode
(
	aiNode* node, const aiScene* scene
)
{
	for (size_t i{ 0 }; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	// Recursion part
	for (size_t i{ 0 }; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh
(
	aiMesh* mesh, const aiScene* scene
)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i{ 0 }; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f,0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i{ 0 }; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j{ 0 }; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh); 
	MeshToText.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials
(
	const aiScene* scene
)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// Position of the back slash
				int idx = std::string(path.data).rfind("\\");
				
				// Copy it after "\\" get rid of local path of the user that exported the texture
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string textPath = "Textures/" + filename;
				textureList[i] = new TNS::Texture(textPath.c_str());

				if (!textureList[i]->loadTexture())
				{
					std::cout << "Failed to load texture at: "<< textPath << std::endl;
					delete textureList[i];
					textureList[i] = nullptr;
				}

			}
		}
	
		if (!textureList[i])
		{
			textureList[i] = new TNS::Texture("Textures/plain.png");
			textureList[i]->loadTexture();
		}
	}
}

void Model::ClearModel()
{
	for (auto& m : meshList)
	{
		if (m != nullptr)
		{
			delete m;
			m = nullptr;
		}	
	}

	for (auto& t : textureList)
	{
		if (t != nullptr)
		{
			delete t;
			t = nullptr;
		}
	}
}

Model::~Model() { ClearModel(); }