#include"GDIRenderTarget.h"

namespace RenderEngine
{
	GDIRenderTarget::GDIRenderTarget(HWND* hWindow,int width,int height) : m_bitmapInfo(*(BITMAPINFO*)&m_bitmapBuf), m_bitmapInfoHeader(m_bitmapInfo.bmiHeader),
	m_width(width), m_height(height), m_frameBufferLength(m_width * m_height), m_hWindow(hWindow)
	{
		m_VPage1 =  nullptr;
		m_ZBuffer2 = nullptr;
		m_VPage1 =  nullptr;
		m_VPage2 =  nullptr;
	}

	GDIRenderTarget::GDIRenderTarget(const GDIRenderTarget& other) :  m_bitmapInfo(other.m_bitmapInfo), m_bitmapInfoHeader(other.m_bitmapInfoHeader),
	m_width(other.m_width), m_height(other.m_height), m_frameBufferLength(other.m_frameBufferLength), m_hWindow(other.m_hWindow)
	{
		m_VPage1 =  nullptr;
		m_ZBuffer2 = nullptr;
		m_VPage1 =  nullptr;
		m_VPage2 =  nullptr;
		this->Initialize();
	}

	GDIRenderTarget::~GDIRenderTarget()
	{}

	bool GDIRenderTarget::Initialize()
	{
		//Create a 32bit color DIB 
		memset(&m_bitmapInfoHeader,0,sizeof(m_bitmapInfoHeader)); //Set m_bitmapInfoHeader to zero
	
		m_bitmapInfoHeader.biSize = sizeof(m_bitmapInfoHeader);
		m_bitmapInfoHeader.biWidth = m_width;
		m_bitmapInfoHeader.biHeight = (-1 * m_height); // DIBs are upside down
		m_bitmapInfoHeader.biPlanes = 1;
		m_bitmapInfoHeader.biBitCount = 32; //Bit per pixel
		m_bitmapInfoHeader.biCompression = BI_BITFIELDS; //No compression
		m_bitmapInfoHeader.biSizeImage = 0; 
		m_bitmapInfoHeader.biXPelsPerMeter = 0;
		m_bitmapInfoHeader.biYPelsPerMeter = 0;
		m_bitmapInfoHeader.biClrUsed = 0;
		m_bitmapInfoHeader.biClrImportant = 0;
	
		//Setup the format our DIB is in

		//32bit color
		((unsigned long*)m_bitmapInfo.bmiColors)[0] = 0x00ff0000;
		((unsigned long*)m_bitmapInfo.bmiColors)[1] = 0x0000ff00;
		((unsigned long*)m_bitmapInfo.bmiColors)[2] = 0x000000ff;

		//Allocate vpage and datapage
		m_VPage1 = new Color[m_frameBufferLength];
		m_VPage2 = new Color[m_frameBufferLength];
		m_ZBuffer1 = new float[m_frameBufferLength];
		m_ZBuffer2 = new float[m_frameBufferLength];
		//Check if this works.
		memset(m_VPage1,0,sizeof(*m_VPage1) * m_frameBufferLength);
		memset(m_VPage2,0,sizeof(*m_VPage2) * m_frameBufferLength);
		
		
		memset(m_ZBuffer1,0.0f,sizeof(float) * m_frameBufferLength);
		memset(m_ZBuffer2,0.0f,sizeof(float) * m_frameBufferLength);

		if(m_VPage1 == NULL)
		{
			return false;
		}

		if(m_ZBuffer1 == NULL)
		{
			return false;
		}

		m_CurrentBackBuffer = m_VPage1; //Set the m_VPage1 as starting backbuffer
		m_CurrentZbuffer = m_ZBuffer1;

		if(m_VPage2 == NULL)
		{
			return false;
		}

		return true;
	}

	void GDIRenderTarget::Shutdown()
	{
		//Check if we allocated memory for the pages.
		if(m_ZBuffer1 != nullptr)
		{
			delete[] m_ZBuffer1;
			m_ZBuffer1 = nullptr;
		}

		if(m_ZBuffer2 != nullptr)
		{
			delete[] m_ZBuffer2;
			m_ZBuffer2 = nullptr;
		} 

		if(m_VPage1 != nullptr)
		{
			delete[] m_VPage1;
			m_VPage1 =  nullptr;
		}

		if(m_VPage2 != nullptr)
		{
			delete[] m_VPage1;
			m_VPage2 =  nullptr;
		}		
	}

	void GDIRenderTarget::Flip()
	{
		RECT client;
		HDC hDC;
		Color *FrontVPData;
		unsigned int *DIBData;
	
		Color BufferOneColor(100, 149, 237,0);
		Color BufferTwoColor(100, 149, 237,0);

		//set up pointers to destination and source
		if(m_CurrentBackBuffer == m_VPage1)
		{
			FrontVPData = m_VPage1; //Set m_VPage1 as frontbuffer we want to present.
			m_CurrentBackBuffer = m_VPage2; //Set m_VPage2 as backbuffer.
			//memset(m_VPage2,0,(sizeof(Color) * m_frameBufferLength));
			SetFrameBufferColor(BufferTwoColor,2); //Set the color of m_vPage2
			m_CurrentZbuffer = m_ZBuffer2;
			memset(m_ZBuffer2,0,(sizeof(float) * m_frameBufferLength)); //Clean ZBuffer2
		}
		else
		{
			FrontVPData = m_VPage2; //Set m_VPage2 as frontBuffer we want to present.
			m_CurrentBackBuffer = m_VPage1; //Set m_vPage2 as backbuffer
			//memset(m_VPage1,0,(sizeof(Color) * m_frameBufferLength));
			SetFrameBufferColor(BufferOneColor,1); //Set the color of m_vPage1 
			m_CurrentZbuffer = m_ZBuffer1; 
			memset(m_ZBuffer1,0,(sizeof(float) * m_frameBufferLength)); //Clean ZBuffer1
		}
	
		DIBData = (unsigned int*)FrontVPData;

		//Get the windows device context
		hDC = GetDC(*(m_hWindow));
	
		//If it's available
		if(hDC != NULL)
		{
			//Get the window dimensions 
			GetClientRect(*(m_hWindow),&client);

			SetDIBitsToDevice(
				hDC,
				client.left,
				client.top,
				m_width,
				m_height,
				0,
				0,
				0,
				m_height,
				DIBData,
				&m_bitmapInfo,
				DIB_RGB_COLORS);
		
			//release the device context.
			ReleaseDC(*(m_hWindow),hDC);
		}
		//Reset background
	}

	Color *GDIRenderTarget::GetColorBuffer() const
	{
		return m_CurrentBackBuffer;
	}
	
	float* GDIRenderTarget::GetDepthBuffer() const
	{
		return m_CurrentZbuffer;;
	}

	int GDIRenderTarget::GetWidth() const
	{
		return m_width;
	}

	int GDIRenderTarget::GetHeight() const
	{
		return m_height;
	}

	void GDIRenderTarget::SetFrameBufferColor(Color color,unsigned int buffNum)
	{
		Color *pixelData = NULL;
		float *DepthData = NULL;

		if(buffNum == 1)
		{
			pixelData = m_VPage1;
			DepthData = m_ZBuffer1;
		}
	
		if(buffNum== 2)
		{
			pixelData = m_VPage2;
			DepthData = m_ZBuffer2;
		}
	
		for (int i=0; i < m_frameBufferLength; i++)
		{
			*pixelData = color;
			*DepthData = 0.0f;
			// next pixel
			pixelData++;
			DepthData++;
		}
	}

}