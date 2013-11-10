// Juul Joosten 2013

#include "Timer.h"
#include "../CoreDefines.h"

namespace Indy
{
	Timer::Timer( void)
	#ifdef WIN32
		:
	m_creationThread(NULL),
	m_start(0),
	m_stop(0),
	m_tickFrequency(0)
	#endif
	{
	#ifdef WIN32
		// get creation thread and store it to be used to set the queryperformance timer on the same thread
		m_creationThread = GetCurrentThread();
	#endif

		// only needed for Windows
		getHighPerformanceCPUFrequency();

	}

	Timer::~Timer( void)
	{

	}

	void Timer::Start( void)
	{
	#ifdef WIN32
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_start);
	#endif
	}

	void Timer::Stop( void)
	{
	#ifdef WIN32
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_stop);
	#endif
	}

	void Timer::getHighPerformanceCPUFrequency( void)
	{
	#ifdef WIN32
		QueryPerformanceFrequency( (LARGE_INTEGER*)&m_tickFrequency);
	#endif
	}


	double Timer::GetSeconds( void) const
	{
		const double seconds = ((double)m_stop - (double)m_start) / (double)m_tickFrequency;
		return seconds;
	}

	double Timer::GetMilliSeconds( void) const
	{
	#ifdef WIN32

		// m_start and m_stop QuadPart returns clock ticks, deviding it by the clock frequency will give clock ticks per second
		const double miliseconds = SECONDS_TO_MILISECONDS( GetSeconds());
		return miliseconds;
	#endif

		// default return
		return 0;
	}

	unsigned long long Timer::GetMicroSeconds( void) const
	{
	#ifdef WIN32
		// m_start and m_stop QuadPart returns clock ticks, deviding it by the clock frequency will give clock ticks per second
		const unsigned long long microseconds = static_cast< unsigned long long>( SECONDS_TO_MICROSECONDS( GetSeconds()));
		return microseconds;
	#endif

		// default return
		return 0;
	}

	unsigned long long Timer::GetClockTicks( void) const
	{
	#ifdef WIN32
		// m_start and m_stop QuadPart returns clock ticks
		return m_stop - m_start;
	#endif

		// default return
		return 0;
	}
}