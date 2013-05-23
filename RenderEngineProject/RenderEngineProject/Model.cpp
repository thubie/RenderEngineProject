#include"Model.h"

namespace RenderEngine
{
	Model::Model()
	{
		vertices = nullptr;
	}

	Model::Model(Vertex* vertices,Face* faces)
	{
		this->vertices = vertices;
		this->faces = faces;
	}

	Model::~Model()
	{
		if(vertices != nullptr)
			delete[] vertices;

	}

	
}