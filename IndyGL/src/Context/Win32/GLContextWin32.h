// Juul Joosten 2013

#pragma once

#include "../GLContext.h"


#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN
	#include <Windows.h>
#endif

namespace Indy
{
	class GLContextWin32 : public GLContext
	{

	public:
		GLContextWin32( const OpenGLInfo& openGLInfo);
		virtual ~GLContextWin32( void);

		bool Create( const HDC const deviceContext,
					 const PIXELFORMATDESCRIPTOR* pixelFormatDiscriptor);

		void Destroy( void);

		// inherited
		void Bind( void);
		void Unbind( void);

		void SwitchHDC( const HDC const newHDC);


	private:
		HGLRC	m_glContextHandle;
		HDC		m_deviceHandle;
	};
}