#pragma once
#include<Windows.h>
#include"RenderTarget.h"

namespace RenderEngine
{
	class GDIRenderTarget : RenderTarget
	{
	public:
		GDIRenderTarget(HWND*,int width,int height);
		GDIRenderTarget(const GDIRenderTarget& other);
		~GDIRenderTarget();

		bool Initialize();
		void Shutdown();
		void Flip();
		Color *GetColorBuffer() const;
		void SetFrameBufferColor(Color,unsigned int buffNum);
		int GetWidth() const;
		int GetHeight() const;
		
	private:
		int m_width;
		int m_height;
		int m_frameBufferLength;
		HWND* m_hWindow; //Handle to the window where we want to present our frame.
		Color* m_VPage1; //Framebuffer 1 for page flipping
		Color* m_VPage2; //Framebuffer 2 for page flipping
		Color* m_CurrentBackBuffer; //FrameBuffer we are working with
		unsigned char m_bitmapBuf[sizeof(BITMAPINFOHEADER) + 12];  //DIBmapBuffer to store bitmapinfoheader.
		BITMAPINFO &m_bitmapInfo;	//Struct to describe bitmapInfo.
		BITMAPINFOHEADER &m_bitmapInfoHeader; //Struct to describe BitmapInfoHeader.
	};
}