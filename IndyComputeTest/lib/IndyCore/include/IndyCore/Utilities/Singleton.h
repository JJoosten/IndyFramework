/*	 Juul Joosten 2013

	--- How to use ---
	- Make sure that you inherit and make Singleton<Class> a friend class
	- Make sure that your deconstructor is private, this will prevent anyone
	  of deleting the returned reference, you can destroy the class by using the
	  Release function, USE THIS FUNCTION WITH CARE!

	--- Example ---
	class A : public Singleton<A> 
	{ 
		friend class Singleton<A>;

	public:
		... do stuff

	private:
		~A();
		
		....
	};
*/

#pragma once

#include "NonCopyable.h"

namespace Indy
{
	template <class T>
	class Singleton : public NonCopyable
	{

	public:
		static void CreateInstance( void)
		{
			assert( m_instance == NULL && "Instance is already created");

#ifdef NDEBUG
			if ( m_instance != NULL)
			{
				// TODO: log message
				return; // unsafe until log files are written when this occurs
			}
#endif

			m_instance = new T();
		}

		static T& GetInstance( void)
		{
			assert( m_instance && "Singleton you request is NULL");

			return *m_instance;
		}
		
		/*	releases the singleton instance
			IMPORTANT: USE WITH CARE!
			WHEN RELEASED, A GetInstance() WILL new THE REQUESTED SINGLETON CLASS! 
		*/
		void Release( void)
		{
			assert( m_instance != NULL && "You are trying to release an singleton that is NULL, recheck your program architecture!");

#ifdef NDEBUG
				// TODO: log message
				return; // unsafe until log files are written when this occurs
#endif
			// the only reason when m_instance could be NULL is when it is released before and the
			// reference is still in use. This will give an assert in the delete (due to delete overloading)
			// if not overloaded, this won't give any problems because deleting NULL is fine by the C++ standards
			delete m_instance;
			m_instance = NULL;
		}


	private:
		static T* m_instance;
	};

	template <class T>
	T* Singleton<T>::m_instance = NULL;
};