#include"VertexProcessor.h"

namespace RenderEngine
{
	VertexProcessor::VertexProcessor()
	{}

	VertexProcessor::VertexProcessor(const VertexProcessor& other) : m_viewTransvorm(other.m_viewTransvorm)
	{}

	void VertexProcessor::SetViewTransMatrix(Matrix4x4 *viewTransMatrix)
	{
		m_viewTransvorm = viewTransMatrix;
	}

	void VertexProcessor::ProcessVertices(Vertex *vertices, int numVertices)
	{
		int i;

		for(i = 0; i < numVertices; ++i)
		{
		}
	}
}