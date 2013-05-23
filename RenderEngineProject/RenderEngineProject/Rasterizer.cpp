////////////////////////////////////////////////////////////////////
//Draw triangle based on this algorithm
//http://devmaster.net/forums/topic/1145-advanced-rasterization/
///////////////////////////////////////////////////////////////////

#include"Rasterizer.h"
#include<Windows.h>

namespace RenderEngine
{

	Rasterizer::Rasterizer(Color* renderTargetBuffer, int stride) : m_renderTargetBuffer(renderTargetBuffer),
		m_stride(stride)
	{}

	Rasterizer::Rasterizer(const Rasterizer &other) : m_renderTargetBuffer(other.m_renderTargetBuffer) ,
		m_stride(other.m_stride)
	{}

	void Rasterizer::SetRenderTargetBuffer(Color* renderTargetBuffer)
	{
		m_renderTargetBuffer = renderTargetBuffer;
	}


	void Rasterizer::DrawTriangle(Vertex *v0, Vertex *v1,Vertex *v2)
	{
		int x,y,xmin,xmax,ymin,ymax;
		
		float x0 = v0->position.m_x;
		float x1 = v1->position.m_x;
		float x2 = v2->position.m_x;
		
		float y0 = v0->position.m_y;
		float y1 = v1->position.m_y;
		float y2 = v2->position.m_y;

		float alpha;
		float beta;
		float gamma;

		Color color;
		Color color1 = v0->diffuse;
		Color color2 = v1->diffuse;
		Color color3 = v2->diffuse;

		//X and y minimal values.
		xmin = min(min(x0,x1),x2);
		xmax = max(max(x0,x1),x2);

		ymin = min(min(y0,y1),y2);
		ymax = max(max(y0,y1),y2);

		int stride = m_stride;
		Color* colorBuffer = m_renderTargetBuffer;
		colorBuffer +=  (ymin * stride);

		//Y-axis constant increment
				
		for(y = ymin; y < ymax; ++y)
		{		
			for(x = xmin; x < xmax; x++)
			{
				alpha = f12(x, y, x1, y1, x2, y2) / f12(x0, x0, x1, y1, x2, y2);
				beta =  f20(x, y, x0, y0, x2, y2) / f20(x1, y1, x0, y0, x2, y2);
				gamma = (1 - alpha) - beta;
				
				if(0 <= alpha && 0 <= beta && 0 <= gamma)
				{
					color.m_r = ((color1.m_r * alpha + color2.m_r * beta + color3.m_r * gamma));
					color.m_g = ((color1.m_g * alpha + color2.m_g * beta + color3.m_g * gamma));
					color.m_b = ((color1.m_b * alpha + color2.m_b * beta + color3.m_b * gamma));
					color.m_a = ((color1.m_a * alpha + color2.m_a * beta + color3.m_a * gamma));
					*(colorBuffer + x) = color;
				}
			}

			colorBuffer += stride;
		}
	}


	inline float Rasterizer::f01(float x, float y, float x0, float y0, float x1, float y1)
	{
		return ((y0 - y1) * x) + ((x1 - x0) * y) + (x0 * y1) - (x1 * y0);
	}

	inline float Rasterizer::f12(float x, float y, float x1, float y1, float x2, float y2)
	{
		return ((y1 - y2) * x) + ((x2 - x1) * y) + (x1 * y2) - (x2 * y1);
	}

	inline float Rasterizer::f20(float x, float y, float x0, float y0, float x2, float y2)
	{
		return ((y2 - y0) * x) + ((x0 - x2) * y) + (x2 * y0) - (x0 * y2);
	}
}


