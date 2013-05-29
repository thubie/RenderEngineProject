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

		const float dx21 = 0.0625f * (X2 - X1);
		const float dx32 = 0.0625f * (X2 - X3);
		const float dx31 = 0.0625f * (X1 - X2);

		const float dy21 = 0.0625f * (Y2 - Y1);
		const float dy32 = 0.0625f * (Y3 - Y2);
		const float dy31 = 0.0625f * (Y3 - Y1);

		//Gradients 
		const float C = -1  / (dx21 * dy31 - dx31 *dy21); //reciprocal of the gradient
		
		//Buffers
		Color* colorBuffer = m_colorBuffer +  miny * m_stride;
		float* zBuffer = m_ZBuffer + miny * m_stride;

		

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

					//float oneOverz = 1 / xZ;
					//float u = (xU / oneOverz );
					//float v = (xV / oneOverz );
				
					//Checkerboard texture
					/*int i1 = (u * 2);
					int i2 = (v * 2);
					int cell = (i1 + i2) % 2;
					if(cell == 0)*/
					colorBuffer[x] = red;
					/*else
						colorBuffer[x] = blue;*/
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
	
	void Rasterizer::DrawTriangleScanline(const Vertex &v1, const Vertex &v2, const Vertex &v3)
	{
		Vertex V1 = v1;
		Vertex V2 = v2;
		Vertex V3 = v3;

		// Sort: V1 top, V2 middle, V3 bottom
		if(V1.position.m_y > V3.position.m_y) 
			SwapVertices(V1, V3);
		if(V2.position.m_y > V3.position.m_y) 
			SwapVertices(V2, V3);
		if(V1.position.m_y > V2.position.m_y) 
			SwapVertices(V1, V2);

		//28.4 fixed point coordinates
		const int Y1 = iround(16.0f * V1.position.m_y);
		const int Y2 = iround(16.0f * V2.position.m_y);
		const int Y3 = iround(16.0f * V3.position.m_y);

		const int X1 = iround(16.0f * V1.position.m_x);
		const int X2 = iround(16.0f * V2.position.m_x);
		const int X3 = iround(16.0f * V3.position.m_x);

		//Deltas
		const int DX12 = X2 - X1; 
		const int DX13 = X3 - X1;
		const int DX23 = X3 - X2;

		const int DY12 = Y2 - Y1; 
		const int DY13 = Y3 - Y1;
		const int DY23 = Y3 - Y2;

		const int FDX12 = DX12 << 4; 
		const int FDX13 = DX13 << 4;
		const int FDX23 = DX23 << 4;

		const int FDY12 = DY12 << 4; 
		const int FDY13 = DY13 << 4;
		const int FDY23 = DY23 << 4;

		const float Dx12 = 0.0625f * DX12;
		const float Dx13 = 0.0625f * DX13;

		const float Dy12 = 0.0625f * DY12;
		const float Dy13 = 0.0625f * DY13;

		//Biggest scan line the one in the on height of Y2
		const int X0 = ceilDiv(DX13 * (Y2 - Y1) + X1 * DY13,DY13);

		//top,middle and bottom
		const int y1 = ceilInt4(Y1);
		const int y2 = ceilInt4(Y2);
		const int y3 = ceilInt4(Y3);

		Color* colorBuffer = m_colorBuffer +  y1 * m_stride;
		//float* zBuffer = zBuffer + y1 * m_stride;

		//Gradient
		const float C = -1 / (Dx12 * Dy13 - Dx13 * Dy12);

		//Depth
		const float Dz12 = V2.position.m_z - V1.position.m_z;
		const float Dz13 = V3.position.m_z - V1.position.m_z;
		float dz_dx = C * (Dz13 * Dy12 - Dz12 * Dy13);
		float dz_dy = C * (Dz12 * Dx13 - Dz13 * Dx12);

		//W value
		const float Dw12 = V2.position.m_z - V1.position.m_z;
		const float Dw13 = V3.position.m_z - V1.position.m_z;
		float dw_dx = C * (Dw13 * Dy12 - Dw12 * Dy13);
		float dw_dy = C * (Dw12 * Dx13 - Dw13 * Dx12);

		//U coord
		const float Du12 = v2.uv.u - v1.uv.u;
		const float Du13 = v3.uv.u - v1.uv.u;
		float du_dx = C * (Du13 * Dy12 - Du12 * Dy13);
		float du_dy = C * (Du12 * Dx13 - Du13 * Dx12);

		//V coord
		const float Dv12 = v2.uv.v - v1.uv.v;
		const float Dv13 = v3.uv.v - v1.uv.v;
		float dv_dx = C * (Dv13 * Dy12 - Dv12 * Dx13);
		float dv_dy = C * (Dv12 * Dy13 - Dv13 * Dx12);

		// Quotient 
		const int Q12 = floorDiv(FDX12, FDY12);
		const int Q13 = floorDiv(FDX13, FDY13);
		const int Q23 = floorDiv(FDX23, FDY23);

		// Remainder
		const int R12 = floorMod(FDX12, FDY12);
		const int R13 = floorMod(FDX13, FDY13);
		const int R23 = floorMod(FDX23, FDY23);

		int ld; int rd;   // error-term
		int lx; int rx;   // start x and end x;   
		int lQ; int rQ;   // edge-step
		int lR; int rR;   // error-step
		int lD; int rD;   // error-overflow

		const int tile = 2;

		//Starting values
		float z;
		float w;

		float u;
		float v;
		
		//Edge step values
		float zS;
		float wS;
		float uS;
		float vS;

		// Prestep
		float Dx;
		float Dy;

		if(X2 < X0)
		{
			lx = ceilDiv(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			ld = ceilMod(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			rx = ceilDiv(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			rd = ceilMod(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			lQ = Q12;   rQ = Q13;
			lR = R12;   rR = R13;
			lD = FDY12; rD = FDY13;
			Dx = 0.0625f * (float)((lx << 4) - X1);
			Dy = 0.0625f * (float)((y1 << 4) - Y1);

			z = v1.position.m_z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v1.position.m_w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			u = v1.uv.u + Dx * du_dx + Dy * du_dy;
			uS = (float)lQ * du_dx + du_dy;

			v = V1.uv.v + Dx * dv_dx + Dy * dv_dy;
			vS = (float)lQ * dv_dx + dv_dy;
		}
		else
		{
			lx = ceilDiv(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			ld = ceilMod(DX13 * (ceilFix4(Y1) - Y1) + X1 * DY13, FDY13);
			rx = ceilDiv(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			rd = ceilMod(DX12 * (ceilFix4(Y1) - Y1) + X1 * DY12, FDY12);
			lQ = Q13;   rQ = Q12;
			lR = R13;   rR = R12;
			lD = FDY13; rD = FDY12;
			Dx = 0.0625f * (float)((lx << 4) - X1);
			Dy = 0.0625f * (float)((y1 << 4) - Y1);

			z = v1.position.m_z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v1.position.m_w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			u = v1.uv.u + Dx * du_dx + Dy * du_dy;
			uS = (float)lQ * du_dx + du_dy;

			v = V1.uv.v + Dx * dv_dx + Dy * dv_dy;
			vS = (float)lQ * dv_dx + dv_dy;
		}

		//Top to middle
		for(int y = y1; y < y2; y++)
		{
			u = uS;
			v = vS;

			for(int x = lx; x < rx; x++)
			{
				Color red(255,0,0,0);
				Color blue(0,0,255,0);
				//Checkerboard texture
				int i1 = (u * tile);
				int i2 = (v * tile);
				int cell = (i1 + i2) % 2;
				if(cell == 0)
					colorBuffer[x] = red;
				else
					colorBuffer[x] = blue;
				u += du_dx;
				v += dv_dx;				
			}
			colorBuffer += m_stride;

			//Increase values
			lx += lQ;
			ld += lR;

			z += zS;
			w += wS;
			
			u += uS;
			v += vS;


			if(ld > 0)
			{
				ld -= lD;
				lx += 1;

				//extra step
				z += dz_dx;
				w += dw_dx; 
				u += du_dx;
				v += dv_dx;
			}

			rx += rQ;
			rd += rR;

			if(rd > 0)
			{
				rd -= rD;
				rx += 1;
			}
		}

		if(X2 < X0)
		{
			lx = ceilDiv(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			ld = ceilMod(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			rx = ceilDiv(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
			rd = ceilMod(DX13 * (ceilFix4(Y2) - Y1) + X1 * DY13, FDY13);
			lQ = Q23;
			lR = R23;
			lD = FDY23;
		
			Dx = 0.0625f * (float)((lx << 4) - X2);
			Dy = 0.0625f * (float)((y2 << 4) - Y2);

			z = v2.position.m_z + Dx * dz_dx + Dy * dz_dy;
			zS = (float)lQ * dz_dx + dz_dy;

			w = v2.position.m_w + Dx * dw_dx + Dy * dw_dy;
			wS = (float)lQ * dw_dx + dw_dy;

			u = v2.uv.u + Dx * du_dx +  du_dy;
			uS = (float)lQ * du_dx + du_dy;

			v = V2.uv.v + Dx * dv_dx +  dv_dy;
			vS = (float)lQ * dv_dx + dv_dy;
		}
		else
		{
			rx = ceilDiv(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);
			rd = ceilMod(DX23 * (ceilFix4(Y2) - Y2) + X2 * DY23, FDY23);

			rQ = Q23;
			rR = R23;
			rD = FDY23;
		}

		//Middle to bottom
		for(int y = y2; y < y3; y++)
		{
			u = uS;
			v = vS;

			for(int x = lx; x < rx; x++)
			{
				Color red(255,0,0,0);
				Color blue(0,0,255,0);
				//Checkerboard texture
				int i1 = (u * tile);
				int i2 = (v * tile);
				int cell = (i1 + i2) % 2;
				if(cell == 0)
					colorBuffer[x] = red;
				else
					colorBuffer[x] = blue;

				u += du_dx;
				v += dv_dx;
			}

			colorBuffer += m_stride;
			lx += lQ;
			ld += lR;
			
			u += uS;
			v += vS;

			if(ld > 0)
			{
				ld -= lD;
				lx += 1;
				
				//extra step
				z += dz_dx;
				w += dw_dx; 
				u += du_dx;
				v += dv_dx;
			}

			rx += rQ;
			rd += rR;

			if(rd > 0)
			{
				rd -= rD;
				rx += 1;
			}
		}
	}

	void Rasterizer::SwapVertices(Vertex& v1,Vertex& v2)
	{
		Vertex temp;
		temp.position = v1.position;
		v1.position = v2.position;
		v2.position = temp.position;

		temp.normal = v1.normal;
		v1.normal = v2.normal;
		v2.normal = temp.normal;

		temp.diffuse = v1.diffuse;
		v1.diffuse = v2.diffuse;
		v2.diffuse = temp.diffuse;

		temp.uv = v1.uv;
		v1.uv = v2.uv;
		v2.uv = temp.uv;
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


