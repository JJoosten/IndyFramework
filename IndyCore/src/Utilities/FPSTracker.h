// Juul Joosten 2013

#pragma once
namespace winterSTD
{
	template <typename T>
	class FPSTracker
	{

	public:
		FPSTracker( void)
			:
		m_miliseconds(0),
		m_currentFPS(0),
		m_currentFPSCounter(0),
		m_sampledFPS(0),
		m_sampleCount(0),
		m_sampledFPSCounter(0)
		{

		}

		~FPSTracker( void)
		{

		}


		void Update( const T deltaMiliSeconds)
		{
			const static T second = 1000;

			// count miliseconds
			m_miliseconds += deltaMiliSeconds;

			// if miliseconds is bigger then second
			if ( m_miliseconds >= second)
			{
				// add sample count so we can track the sampled FPS overtime
				++m_sampleCount;

				// save current and sampled FPS
				m_currentFPS = m_currentFPSCounter;
				m_sampledFPS = m_sampledFPSCounter / m_sampleCount;

				m_miliseconds -= second;

				// reset current FPS counter
				m_currentFPSCounter = 0;
			}

			m_currentFPSCounter++;
			m_sampledFPSCounter++;
		}


		// Getters & Setters
		T GetCurrentFPS( void) const
		{
			return m_currentFPS;
		}

		T	GetSampledFPS( void) const
		{
			return m_sampledFPS;
		}
		

	private:
		T	m_miliseconds;
		T	m_currentFPS;
		T	m_currentFPSCounter;
		T	m_sampledFPS;
		T	m_sampleCount;
		T	m_sampledFPSCounter;
	};
};