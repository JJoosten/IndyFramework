// Juul Joosten 2013

#pragma once

#ifdef WIN32
	#include <Windows.h>
#else 
	#error NO SUPPORT! 
#endif


namespace Indy
{
	class Timer
	{

	public:
		Timer( void);
		~Timer( void);

		void Start( void);
		void Stop( void);

		double GetSeconds( void) const;

		// returns the miliseconds between start and stop, beware that on small timer steps this could give 0 
		double GetMilliSeconds( void) const;
		
		// returns the microseconds between start and stop
		unsigned long long GetMicroSeconds( void) const;

		// returns the clock ticks between start and stop
		unsigned long long GetClockTicks( void) const;

	private:
		// gets the cpu frequency for the queryperformancetimer on windows
		void getHighPerformanceCPUFrequency( void);

	public:
#ifdef WIN32
		HANDLE				m_creationThread;
		unsigned long long	m_start;
		unsigned long long	m_stop;
		unsigned long long	m_tickFrequency;
#endif
	};
};
