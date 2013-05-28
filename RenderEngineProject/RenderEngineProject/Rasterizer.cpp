////////////////////////////////////////////////////////////////////
//Draw triangle based on this algorithm
//http://devmaster.net/forums/topic/1145-advanced-rasterization/
///////////////////////////////////////////////////////////////////

#include"Rasterizer.h"


namespace RenderEngine
{

	Rasterizer::Rasterizer(Color* renderTargetBuffer, int stride) : m_colorBuffer(renderTargetBuffer),
		m_stride(stride)
	{}

	Rasterizer::Rasterizer(const Rasterizer &other) : m_colorBuffer(other.m_colorBuffer) ,
		m_stride(other.m_stride)
	{}

	void Rasterizer::SetRenderTargetBuffer(Color* renderTargetBuffer)
	{
		m_colorBuffer = renderTargetBuffer;
	}

	void Rasterizer::SetZBuffer(float *ZBuffer)
	{
		m_ZBuffer = ZBuffer;
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

		*(m_colorBuffer + y * m_stride + x) = color;

		if (dx >= dy)
		{
			dy <<= 1;
			balance = dy - dx;
			dx <<= 1;

			while (x != x2)
			{
				//SetPixel(color,x,y);
				*(m_colorBuffer + y * m_stride + x) = color;
				if (balance >= 0)
				{
					y += incy;
					balance -= dx;
				}
				balance += dy;
				x += incx;
			} 
			*(m_colorBuffer + y * m_stride + x) = color;
		}
		else
		{
			dx <<= 1;
			balance = dx - dy;
			dy <<= 1;

			while (y != y2)
			{
				*(m_colorBuffer + y * m_stride + x) = color;
				if (balance >= 0)
				{
					x += incx;
					balance -= dy;
				}
				balance += dx;
				y += incy;
			} 
			*(m_colorBuffer + y * m_stride + x) = color;
		}
		*(m_colorBuffer + y * m_stride + x) = color;
	}

	void Rasterizer::DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		// 28.4 fixed-point coordinates
		const int Y1 = iround(16.0f * v1.position.m_y);
		const int Y2 = iround(16.0f * v2.position.m_y);
		const int Y3 = iround(16.0f * v3.position.m_y);

		const int X1 = iround(16.0f * v1.position.m_x);
		const int X2 = iround(16.0f * v2.position.m_x);
		const int X3 = iround(16.0f * v3.position.m_x);

		// Deltas
		const int DX12 = X1 - X2;
		const int DX23 = X2 - X3;
		const int DX31 = X3 - X1;

		const int DY12 = Y1 - Y2;
		const int DY23 = Y2 - Y3;
		const int DY31 = Y3 - Y1;

		// Fixed-point deltas
		const int FDX12 = DX12 << 4;
		const int FDX23 = DX23 << 4;
		const int FDX31 = DX31 << 4;

		const int FDY12 = DY12 << 4;
		const int FDY23 = DY23 << 4;
		const int FDY31 = DY31 << 4;

		// Bounding rectangle
		int minx = (min(X1, X2, X3) + 0xF) >> 4;
		int maxx = (max(X1, X2, X3) + 0xF) >> 4;
		int miny = (min(Y1, Y2, Y3) + 0xF) >> 4;
		int maxy = (max(Y1, Y2, Y3) + 0xF) >> 4;
		
		Color* colorBuffer = m_colorBuffer +  miny * m_stride;
		float* zBuffer;
		zBuffer = m_ZBuffer + miny * m_stride;
		// Half-edge constants
		int C1 = DY12 * X1 - DX12 * Y1;
		int C2 = DY23 * X2 - DX23 * Y2;
		int C3 = DY31 * X3 - DX31 * Y3;

		// Correct for fill convention
		if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
		if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
		if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;

		int CY1 = C1 + DX12 * (miny << 4) - DY12 * (minx << 4);
		int CY2 = C2 + DX23 * (miny << 4) - DY23 * (minx << 4);
		int CY3 = C3 + DX31 * (miny << 4) - DY31 * (minx << 4);

		float alpha;
		float beta;
		float gamma;

		for(int y = miny; y < maxy; y++)
		{
			int CX1 = CY1;
			int CX2 = CY2;
			int CX3 = CY3;

			for(int x = minx; x < maxx; x++)
			{
				if(CX1 > 0 && CX2 > 0 && CX3 > 0)
				{
					Color red(255,0,0,0);
					Color blue(0,0,255,0);

					float norm = (CX1) + (CX2) + (CX3);
					alpha = CX1  / norm;
					beta  = CX2  / norm;
					gamma = CX3  / norm;
					float d = (v2.perW * v3.perW) + (( v3.perW * beta) * (v1.perW - v2.perW)) +(( v2.perW * gamma) * (v1.perW - v3.perW));
					float perwbeta  = (v1.perW * v3.perW * beta) / d;
					float perwgamma = (v1.perW * v2.perW * gamma) / d;
					float perwalpha  = 1 - perwbeta - perwgamma; 
					float u = (v1.uv.u * perwalpha ) + (v2.uv.u * perwbeta ) + (v3.uv.u * perwgamma);
					float v = (v1.uv.v * perwalpha ) + (v2.uv.v * perwbeta ) + (v3.uv.v * perwgamma);
					//u = u + 0.5;
					//v = v + 0.5;

					//Checkerboard texture
					int i1 = ifloor(u * 2);
					int i2 = ifloor(v * 2);
					int cell = (i1 + i2) % 2;
					if(cell == 0)
						colorBuffer[x] = red;
					else
						colorBuffer[x] = blue;
					/*colorBuffer[x].m_r = (v1.diffuse.m_r * alpha + v2.diffuse.m_r * beta + v3.diffuse.m_r * gamma)/ perw;
					colorBuffer[x].m_g = (v1.diffuse.m_g * alpha + v2.diffuse.m_g * beta + v3.diffuse.m_g * gamma)/ perw;
					colorBuffer[x].m_b = (v1.diffuse.m_b * alpha + v2.diffuse.m_b * beta + v3.diffuse.m_b * gamma)/ perw;
					colorBuffer[x].m_a = (v1.diffuse.m_a * alpha + v2.diffuse.m_a * beta + v3.diffuse.m_a * gamma)/ perw;*/
				}
				CX1 -= FDY12;
				CX2 -= FDY23;
				CX3 -= FDY31;
			}
			CY1 += FDX12;
			CY2 += FDX23;
			CY3 += FDX31;
			colorBuffer += m_stride;
			zBuffer += m_stride;
		}
	}

	void Rasterizer::DrawTriangleFloat(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		float y1 = v1.position.m_y;
		float y2 = v2.position.m_y;
		float y3 = v3.position.m_y;

		float x1 = v1.position.m_x;
		float x2 = v2.position.m_x;
		float x3 = v3.position.m_x;

		// Deltas
		float Dx12 = x1 - x2;
		float Dx23 = x2 - x3;
		float Dx31 = x3 - x1;

		float Dy12 = y1 - y2;
		float Dy23 = y2 - y3;
		float Dy31 = y3 - y1;

		// Bounding rectangle
		int minx = (int)min(x1, x2, x3);
		int maxx = (int)max(x1, x2, x3);
		int miny = (int)min(y1, y2, y3);
		int maxy = (int)max(y1, y2, y3);

		Color* colorBuffer = m_colorBuffer +  miny * m_stride;
		float* zBuffer;
		zBuffer = m_ZBuffer + miny * m_stride;

		// Constant part of half-edge functions
		float C1 = Dy12 * x1 - Dx12 * y1;
		float C2 = Dy23 * x2 - Dx23 * y2;
		float C3 = Dy31 * x3 - Dx31 * y3;

		float Cy1 = C1 + Dx12 * miny - Dy12 * minx;
		float Cy2 = C2 + Dx23 * miny - Dy23 * minx;
		float Cy3 = C3 + Dx31 * miny - Dy31 * minx;

		double alpha;
		double beta;
		double gamma;

		// Scan through bounding rectangle
		for(int y = miny; y < maxy; y++)
		{
			// Start value for horizontal scan
			float Cx1 = Cy1;
			float Cx2 = Cy2;
			float Cx3 = Cy3;

			for(int x = minx; x < maxx; x++)
			{
				if(Cx1 > 0 && Cx2 > 0 && Cx3 > 0)
				{
					double norm = (Cx1 + Cx2 + Cx3) + EPSILON;
					alpha = Cx1 / norm;
					beta = Cx2 / norm;
					gamma = Cx3 / norm;
					float total = alpha + beta + gamma;
					char color = min(255 * (((v1.position.m_z * alpha) + (v2.position.m_z * beta) + (v3.position.m_z * gamma)) *  0.5f),255.0);
					colorBuffer[x] = Color(color,color,color,255); 
				}
				Cx1 -= Dy12;
				Cx2 -= Dy23;
				Cx3 -= Dy31;
			}
			Cy1 += Dx12;
			Cy2 += Dx23;
			Cy3 += Dx31;


			colorBuffer += m_stride;
		}
	}

	void Rasterizer::DrawTriangleLine(const Vertex& v1, const Vertex& v2, const Vertex& v3)
	{
		
		int x1 = v1.position.m_x;
		int	x2 = v2.position.m_x;
		int	x3 = v3.position.m_x;

		int y1 = v1.position.m_y;
		int y2 = v2.position.m_y;
		int y3 = v3.position.m_y;

		Color line(0,0,0,0);
		DrawLine(x1,y1,x2,y2,line);
		DrawLine(x2,y2,x3,y3,line);
		DrawLine(x3,y3,x1,y1,line);
	}
	
	float Rasterizer::Orient2D(const float x1, const float y1, const float x2, const float y2,const float x3, const float y3)
	{
		return ((x2 - x1) * (y3 - y1)) - ((y2 - y1) * (x3 - x1));
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


