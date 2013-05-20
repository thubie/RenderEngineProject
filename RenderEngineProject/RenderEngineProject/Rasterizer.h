#pragma once

#include"Color.h"
#include"Vertex.h"
#include"LinearMath.h"

namespace RenderEngine
{
	class Rasterizer
	{
	public:
		Rasterizer(Color* renderTarget, int stride);
		Rasterizer(const Rasterizer &other);
		void SetRenderTargetBuffer(Color* renderTarget);
		void DrawLine(int x1,int y1,int x2,int y2,Color color);
		void DrawTriangle(Vertex *v1, Vertex *v2,Vertex *v3);

	private:

		Color* m_renderTargetBuffer;
		int m_stride;
	};
}