#pragma once

#include"LinearMath.h"
#include"Clipper.h"
#include"Vertex.h"
namespace RenderEngine
{
	class VertexProcessor
	{
	public:
		VertexProcessor();
		VertexProcessor(const VertexProcessor& other);
 
		void SetViewTransMatrix(Matrix4x4 *viewTransMatrix);
		void ProcessVertices(Vertex *vertices, int numVertices);

	private:
		void ConstructViewTransform();

	private:
		Matrix4x4* m_viewTransvorm;
		
		//IndexBuffer* m_indexBuffer;
		//VertexBuffer* m_indexBuffer;
	};
}