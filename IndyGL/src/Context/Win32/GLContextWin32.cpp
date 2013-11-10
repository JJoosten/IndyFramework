// Juul Joosten 2013

#include "GLContextWin32.h"

#include <IndyCore/CoreDefines.h>
#include "GL/wglew.h"

namespace Indy
{

	GLContextWin32::GLContextWin32( const OpenGLInfo& openGLInfo)
		:
	GLContext(openGLInfo),
	m_glContextHandle(NULL),
	m_deviceHandle(NULL)
	{

	}

	GLContextWin32::~GLContextWin32( void)
	{
		// delete context
		if( !wglDeleteContext(m_glContextHandle))
			BREAKPOINT(GLContext could not be deleted properly);


	}

	bool GLContextWin32::Create( const HDC const deviceContext, 
								 const PIXELFORMATDESCRIPTOR* pixelFormatDiscriptor)
	{
		m_deviceHandle = deviceContext;

		// default pixel format descriptor?
		if ( pixelFormatDiscriptor == NULL)
		{
			// setup pixel format
			PIXELFORMATDESCRIPTOR pfd =
			{   
				sizeof(PIXELFORMATDESCRIPTOR),  // size
				1,                          // version
				PFD_SUPPORT_OPENGL |        // OpenGL window
				PFD_DRAW_TO_WINDOW |        // render to window
				PFD_DOUBLEBUFFER,           // support double-buffering
				PFD_TYPE_RGBA,              // color type
				32,                         // prefered color depth
				0, 0, 0, 0, 0, 0,           // color bits (ignored)
				0,                          // no alpha buffer
				0,                          // alpha bits (ignored)
				0,                          // no accumulation buffer
				0, 0, 0, 0,                 // accum bits (ignored)
				32,                         // depth buffer
				0,                          // no stencil buffer
				0,                          // no auxiliary buffers
				PFD_MAIN_PLANE,             // main layer
				0,                          // reserved
				0, 0, 0,                    // no layer, visible, damage masks
			};
			pixelFormatDiscriptor = &pfd;
		}

		int pixelFormat;
		if (!(pixelFormat = ChoosePixelFormat( deviceContext, pixelFormatDiscriptor))) // match requested pixelformat to an appropriate pixel format
		{
			printf("Cant find suitable Pixel Format.");
			return false;
		}

		if (!SetPixelFormat( deviceContext, pixelFormat, pixelFormatDiscriptor)) 
		{
			printf("Cant setup right pixel format.");
			return false;
		}

		GLContext* currentBound = m_currentGLContextBound;

		HGLRC tempContext = wglCreateContext( deviceContext);
		wglMakeCurrent( deviceContext, tempContext);


		// request forwardable context
		if(m_openGLInfo.MajorVersion >= 3)
		{	
			// get forward compatible context function pointer
			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB"); 

			// check if OpenGL3 is supported
			if( !wglCreateContextAttribsARB)
			{
				// cleanup temp context
				wglDeleteContext( tempContext);

				printf("OpenGL 3 is not supported");

				return false;
			}

			// create OpenGL3 context
			int attributes[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, (int)m_openGLInfo.MajorVersion,
								 WGL_CONTEXT_MINOR_VERSION_ARB, (int)m_openGLInfo.MinorVersion,
								 WGL_CONTEXT_FLAGS_ARB, 0,0};/*WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,  
								 WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
								 0};*/

			// create OpenGL 3 render context
			m_glContextHandle = wglCreateContextAttribsARB( deviceContext, 0, attributes);

			if( !m_glContextHandle)
			{
				m_glContextHandle = tempContext;
				printf("Fallback to < 3.0 context");
		
				glGetIntegerv(GL_MAJOR_VERSION, (GLint*)&m_openGLInfo.MajorVersion);
				glGetIntegerv(GL_MINOR_VERSION, (GLint*)&m_openGLInfo.MinorVersion);

				return false;
			}
			else
			{
				wglDeleteContext(tempContext);
			}
		}
		else
		{
			m_glContextHandle = tempContext;
		}

		if( currentBound != NULL)
			currentBound->Bind();

		glGetIntegerv(GL_MAJOR_VERSION, (GLint*)&m_openGLInfo.MajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, (GLint*)&m_openGLInfo.MinorVersion);


		return true;
	}

	void GLContextWin32::Bind( void)
	{
		if( !wglMakeCurrent( m_deviceHandle, m_glContextHandle))
			BREAKPOINT(wglMakeCurrent gave error!);

		m_currentGLContextBound = this;
	}

	void GLContextWin32::UnBind( void)
	{
		wglMakeCurrent( NULL, NULL);
		m_currentGLContextBound = NULL;
	}

	void GLContextWin32::SwitchHDC( const HDC const newHDC)
	{
		// automaticly binds new HDC
		m_deviceHandle = newHDC;
		Bind();
	}
}