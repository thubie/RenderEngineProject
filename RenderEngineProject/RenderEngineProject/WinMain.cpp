/////////////////////////////////////////////
//Author:Thubie de Jong
//Date: 5/18/2013
//Description: WinMain entry point
////////////////////////////////////////////


#include "WindowApp.h"


using namespace RenderEngine;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pSCmdLine,int iCmdshow)
{
	WindowsApp* windowsApp;
	bool result;

	//Create the windowClass object
	windowsApp = new WindowsApp();
	
	if(!windowsApp)
	{
		return 0;
	}

	//Initialize and check it then run 
	result = windowsApp->Initialize();
	if(result)
	{
		windowsApp->Run();
	}
	windowsApp->Shutdown();
	delete windowsApp;
	windowsApp = 0;
	return 0;
}

