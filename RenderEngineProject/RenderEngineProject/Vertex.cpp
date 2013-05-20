#include"Vertex.h"

namespace RenderEngine
{
	Vertex::Vertex() : m_position(Vector4D(0,0,0,1)), m_diffuse(Color(0,0,0,0)), m_perW(1)
	{}

	Vertex::Vertex(Vector4D position,Color diffuse) : m_position(position), m_diffuse(diffuse), m_perW(1)
	{}

	Vertex::Vertex(const Vertex& other) : m_position(other.m_position), m_diffuse(other.m_diffuse), m_perW(other.m_perW)
	{}
}