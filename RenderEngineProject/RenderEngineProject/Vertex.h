#pragma once

#include"LinearMath.h"
#include"Color.h"

namespace RenderEngine
{
	struct TexCoord
	{
		float u;
		float v;
	};

	struct Vertex
	{
	public:
		Vertex()
		{
			perH = 1.0f;
		};
		Vector4D position;
		Vector3D normal;
		Color  diffuse;
		TexCoord uv;
		float perH;
	};
	
}
