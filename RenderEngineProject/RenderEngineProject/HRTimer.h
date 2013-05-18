#pragma once

#include<Windows.h>

namespace RenderEngine
{
	class HRTimer
	{
	public:
		HRTimer()
		{
			m_Start.QuadPart = m_Stop.QuadPart = 0;
		}
		~HRTimer()
		{}

		// Starts the timer
		void Start()
		{
			::QueryPerformanceCounter(&m_Start);
		}

		// Stops the timer
		void Stop()
		{
			::QueryPerformanceCounter(&m_Stop);
		}

		// Returns the counter at the last Start()
		LONGLONG GetStartCounter()
		{
			return m_Start.QuadPart;
		}

		// Returns the counter at the last Stop()
		LONGLONG GetStopCounter()
		{
			return m_Stop.QuadPart;
		}

		// Returns the interval between the last Start() and Stop()
		LONGLONG GetElapsed()
		{
			return (m_Stop.QuadPart - m_Start.QuadPart);
		}

		// Returns the interval between the last Start() and Stop() in seconds
		double GetElapsedAsSeconds()
		{
			LARGE_INTEGER liFrequency;
			::QueryPerformanceFrequency(&liFrequency);
			return ((double)GetElapsed() / (double)liFrequency.QuadPart);
		}
	private:
		LARGE_INTEGER m_Start,m_Stop;
	};

}