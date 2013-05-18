///////////////////////////////////////////////////////////
//Author:Thubie de Jong
//Date: 5/18/2013
//File: WindowsApp.h
//Description: Handle Input
//		
//
////////////////////////////////////////////////////////////
#pragma once 

namespace RenderEngine
{
	class InputHandler
	{
	public:
		InputHandler();
		InputHandler(const InputHandler& other);
		~InputHandler();

		void Initialize();

		void KeyDown(unsigned int);
		void KeyUp(unsigned int);

		bool IsKeyDown(unsigned int);

	private:
		bool m_keys[256];
	};
	
}
