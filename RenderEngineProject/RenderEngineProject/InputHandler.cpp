#include"InputHandler.h"

namespace RenderEngine
{
	InputHandler::InputHandler()
	{}

	InputHandler::InputHandler(const InputHandler& other)
	{}

	InputHandler::~InputHandler()
	{}

	//Initialize the inputhandler
	void InputHandler::Initialize()
	{
		int i;

		//Initialize all the keys to being released and not pressed.
		for(i = 0; i < 256; ++i)
		{
			m_keys[i] = false;
		}

		return;
	}

	void InputHandler::KeyDown(unsigned int input)
	{
		//if a key is pressed then save that state in the key array.
		m_keys[input] = true;
		return;
	}

	void InputHandler::KeyUp(unsigned int input)
	{
		//If a key is released then clear that state in the key array.
		m_keys[input] = false;
		return;
	}

	bool InputHandler::IsKeyDown(unsigned int key)
	{
		//Return what state the key is in(pressed/not pressed).
		return m_keys[key];
	}
}

