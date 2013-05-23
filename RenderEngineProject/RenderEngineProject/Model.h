#pragma once
#include"Vertex.h"
#include"Matrix4x4.h"


namespace RenderEngine
{
	struct Face
	{
		int indices[3];
	};

	struct Model
	{		
		Vertex* vertices;
		Face* faces;
		int faceCount;
	};

}