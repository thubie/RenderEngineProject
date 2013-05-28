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
		void SetRenderTargetBuffer(Color* colorBuffer);
		void SetZBuffer(float *ZBuffer);
		void DrawLine(int x1,int y1,int x2,int y2,Color color);
		void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3);
		void DrawTriangleFloat(const Vertex &v1, const Vertex &v2, const Vertex &v3);
		void DrawTriangleLine(const Vertex &v1, const Vertex &v2, const Vertex &v3);
		
		
	private:
		//Barycentric calculations
		float f01(float x, float y, float x0, float y0, float x1, float y1);
		float f12(float x, float y, float x0, float y0, float x1, float y1);
		float f20(float x, float y, float x0, float y0, float x1, float y1);
		float Orient2D(const float x1, const float y1, const float x2, const float y2,const float x3, const float y3);

		Color* m_colorBuffer;
		float* m_ZBuffer;
		int m_stride;
	};
}