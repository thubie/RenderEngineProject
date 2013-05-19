#include"Rasterizer.h"

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

	void Rasterizer::DrawTriangle()
	{}


}