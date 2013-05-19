#pragma once

# include"Color.h"

namespace RenderEngine
{
	class Rasterizer
	{
	public:
		Rasterizer(Color* renderTarget, int stride);
		Rasterizer(const Rasterizer &other);
		void SetRenderTargetBuffer(Color* renderTarget);
		void DrawLine(int x1,int y1,int x2,int y2,Color color);
		//Implement This later..
		void DrawTriangle(/*vertex1,vertex2,vertex3*/);
	private:

		Color* m_renderTargetBuffer;
		int m_stride;
	};
}