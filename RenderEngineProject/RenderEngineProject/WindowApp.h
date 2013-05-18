///////////////////////////////////////////////////////////
//Author:Thubie de Jong
//Date: 5/18/2013
//File: WindowsApp.h
//Description: Win32 application here we initialize 
//			   and encapsulate the win32 creation process
////////////////////////////////////////////////////////////

#pragma once 

//Minimalistic windows
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include"InputHandler.h"
#include"GDIRenderTarget.h"

namespace RenderEngine
{
	class WindowsApp
	{
	public:
		WindowsApp();
		WindowsApp(const WindowsApp& other);
		~WindowsApp();

		bool Initialize();
		void Shutdown();
		void Run();

		LRESULT CALLBACK MessageHandler(HWND,UINT,WPARAM,LPARAM);

	private:
		bool NextFrame();
		void InitializeWindows(int& width, int& height);
		void ShutdownWindows();

	private:
		LPCWSTR m_applicationName;
		HINSTANCE m_hinstance;
		HWND m_hwnd;

		int m_frameWidth;
		int m_frameHeight;

		InputHandler* m_Input;
		GDIRenderTarget* m_RenderTarget;
	};

	static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

	static WindowsApp* ApplicationHandle = 0;
}

