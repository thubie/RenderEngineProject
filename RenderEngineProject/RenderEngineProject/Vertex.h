#pragma once

#include"LinearMath.h"
#include"Color.h"

namespace RenderEngine
{
	
	struct Vertex
	{
		Vertex();
		Vertex(Vector4D position,Color diffuse);
		Vertex(const Vertex& other);

		Vector4D m_position;
		Color m_diffuse;
		float m_perW;
	};
	
}
