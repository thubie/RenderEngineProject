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

	void Rasterizer::DrawLine(int x1,int y1,int x2,int y2,Color color)
	{
		int dx,dy;
		int balance;
		int incy,incx;
		int x,y;

		if (x2 >= x1)
		{
			dx = x2 - x1;
			incx = 1;
		}
		else
		{
			dx = x1 - x2;
			incx = -1;
		}

		if (y2 >= y1)
		{
			dy = y2 - y1;
			incy = 1;
		}
		else
		{
			dy = y1 - y2;
			incy = -1;
		}

		x = x1;
		y = y1;

		*(m_renderTargetBuffer + y * m_stride + x) = color;

		if (dx >= dy)
		{
			dy <<= 1;
			balance = dy - dx;
			dx <<= 1;

			while (x != x2)
			{
				//SetPixel(color,x,y);
				*(m_renderTargetBuffer + y * m_stride + x) = color;
				if (balance >= 0)
				{
					y += incy;
					balance -= dx;
				}
				balance += dy;
				x += incx;
			} 
			*(m_renderTargetBuffer + y * m_stride + x) = color;
		}
		else
		{
			dx <<= 1;
			balance = dx - dy;
			dy <<= 1;

			while (y != y2)
			{
				*(m_renderTargetBuffer + y * m_stride + x) = color;
				if (balance >= 0)
				{
					x += incx;
					balance -= dy;
				}
				balance += dx;
				y += incy;
			} 
			*(m_renderTargetBuffer + y * m_stride + x) = color;
		}
		*(m_renderTargetBuffer + y * m_stride + x) = color;
	}

	void Rasterizer::DrawTriangle(Vertex *v1, Vertex *v2,Vertex *v3)
	{
		float x1 = v1->m_position.m_x;
		float x2 = v2->m_position.m_x;
		float x3 = v3->m_position.m_x;
		
		float y1 = v1->m_position.m_y;
		float y2 = v2->m_position.m_y;
		float y3 = v3->m_position.m_y;
		
		// Deltas
		float Dx12 = x1 - x2;
		float Dx23 = x2 - x3;
		float Dx31 = x3 - x1;

		float Dy12 = y1 - y2;
		float Dy23 = y2 - y3;
		float Dy31 = y3 - y1;

		int x,y;
		Color color;
		//bounding rectangle
		int xmin = min(min(x1,x2),x3);
		int xmax = max(max(x1,x2),x3);
		int ymin = min(min(y1,y2),y3);
		int ymax = max(max(y1,y2),y3);
		
		// Constant part of half-edge functions
		float C1 = Dy12 * x1 - Dx12 * y1;
		float C2 = Dy23 * x2 - Dx23 * y2;
		float C3 = Dy31 * x3 - Dx31 * y3;

		float Cy1 = C1 + Dx12 * ymin - Dy12 * xmin;
		float Cy2 = C2 + Dx23 * ymin - Dy23 * xmin;
		float Cy3 = C3 + Dx31 * ymin - Dy31 * xmin;

		int stride = m_stride;
		Color* colorBuffer = m_renderTargetBuffer + (ymin * stride);
		//colorBuffer += (ymin * stride);

		for(y = ymin; y < ymax; ++y)
		{
			 // Start value for horizontal scan
			float Cx1 = Cy1;
			float Cx2 = Cy2;
			float Cx3 = Cy3;

			for(x = xmin; x < xmax; x++)
			{
				if(Cx1 >= 0 && Cx2 >= 0 && Cx3 > 0)
				{
					*(colorBuffer + x) = v1->m_diffuse;
				}

				Cx1 -= Dy12;
				Cx2 -= Dy23;
				Cx3 -= Dy31;
			}

			Cy1 += Dx12;
			Cy2 += Dx23;
			Cy3 += Dx31;

			colorBuffer += stride;
		}
	}
}


