///////////////////////////////////////////////////////////
//Author:Thubie de Jong
//Date: 5/18/2013
//File: WindowsApp.cpp
//Description: Win32 application here we initialize 
//			   and encapsulate the win32 creation process
////////////////////////////////////////////////////////////

#include"WindowApp.h"

namespace RenderEngine
{

	WindowsApp::WindowsApp() : m_frameWidth(1280), m_frameHeight(720)
	{
		//Set the member class pointers to zero
		m_Input = nullptr; 
		m_RenderTarget = nullptr;
	}

	WindowsApp::WindowsApp(const WindowsApp& other) :
		m_frameWidth(other.m_frameWidth),m_frameHeight(other.m_frameHeight)
	{}

	WindowsApp::~WindowsApp()
	{}

	bool WindowsApp::Initialize()
	{
		int screenWidth, screenHeight;
		bool result;

		//Initialize the width and height of the screen to zero 
		screenWidth = 0;
		screenHeight = 0;

		//Initialize the windows api
		InitializeWindows(screenWidth,screenHeight);

		// Create the input object.  This object will be used to handle reading the keyboard input from the user.
		m_Input = new InputHandler;
		if(m_Input == nullptr)
			return false;
		
		// Initialize the input object.
		m_Input->Initialize();

		//Create the renderer object
		m_renderer = new Renderer(&m_hwnd,m_frameWidth,m_frameHeight);
		if(m_renderer == nullptr)
			return false;

		result = m_renderer->Initialize();
		if(!result)
			return false; //if initialize failed

		return true;
	}

	void WindowsApp::Shutdown()
	{
		if(m_renderer != 0)
		{
			m_renderer->Shutdown();
			delete m_renderer;
			m_renderer = 0;
		}

		//Release member class objects here
		if(m_Input != 0)
		{
			delete m_Input;
			m_Input = 0;
		}

		//Shutdown the window.
		ShutdownWindows();
	}

	void WindowsApp::Run()
	{
		MSG msg;
		bool done,result;

		//Initialize the message structure.
		ZeroMemory(&msg,sizeof(MSG));

		//Loop until there is a quit message from the window or the user.
		done = false;
		while(!done)
		{
			//Handle the windows messages.
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//if windows signals to end the application then exit out.
			if(msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				//Otherwise generate new frame
				result = NextFrame();
				if(!result)
				{
					//if failed to generate a new frame quit
					done = true;
				}
			}
		}//end while

		return;
	}

	//Generate a new frame
	bool WindowsApp::NextFrame()
	{
		bool result;

		//Check if the user pressed escape and wants to exit the application.
		if(m_Input->IsKeyDown(VK_ESCAPE))
			return false;
		
		result = m_renderer->NextFrame();
		if(!result)
			return false; //Rendering the next frame failed.
		
		return true;
	}

	//Handle other messages from the message pump
	LRESULT CALLBACK WindowsApp::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch(umsg)
		{
			// Check if a key has been pressed on the keyboard.
			case WM_KEYDOWN:
			{
				// If a key is pressed send it to the input object so it can record that state.
				m_Input->KeyDown((unsigned int)wparam);
				return 0;
			}

			// Check if a key has been released on the keyboard.
			case WM_KEYUP:
			{
				// If a key is released then send it to the input object so it can unset the state for that key.
				m_Input->KeyUp((unsigned int)wparam);
				return 0;
			}

			// Any other messages send to the default message handler as our application won't make use of them.
			default:
			{
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
	}

	void WindowsApp::InitializeWindows(int& screenWidth, int& screenHeight)
	{
		WNDCLASSEX wc;
		int posX, posY;

		// Get an external pointer to this object.	
		ApplicationHandle = this;

		// Get the instance of this application.
		m_hinstance = GetModuleHandle(NULL);

		// Give the application a name.
		m_applicationName = L"RenderEngine";

		// Setup the windows class with default settings.
		memset(&wc,0,sizeof(wc));
		wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = m_hinstance;
		wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm       = wc.hIcon;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = m_applicationName;
		wc.cbSize        = sizeof(WNDCLASSEX);
	
		// Register the window class.
		RegisterClassEx(&wc);
	
		//set it to 1280x720 resolution.
		screenWidth  = 1280;
		screenHeight = 720;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
		

		// Create the window with the screen settings and get the handle to it.
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
			WS_OVERLAPPEDWINDOW,posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

		// Bring the window up on the screen and set it as main focus.
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);

		return;
	}

	void WindowsApp::ShutdownWindows()
	{
		// Show the mouse cursor.
		ShowCursor(true);

		// Remove the window.
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;

		// Remove the application instance.
		UnregisterClass(m_applicationName, m_hinstance);
		m_hinstance = NULL;

		// Release the pointer to this class.
		ApplicationHandle = NULL;

		return;
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
	{
		switch(umessage)
		{
			// Check if the window is being destroyed.
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

			// Check if the window is being closed.
			case WM_CLOSE:
			{
				PostQuitMessage(0);		
				return 0;
			}

			// All other messages pass to the message handler in the system class.
			default:
			{
				return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
			}
		}
	}

}

