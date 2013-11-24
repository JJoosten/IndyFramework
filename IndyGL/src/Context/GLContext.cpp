// Juul Joosten 2013

#include "GLContext.h"
#include <IndyCore/CoreDefines.h>

#include "GL/wglew.h"

namespace Indy
{
	// static members
	std::vector<GLContext*> GLContext::m_contexts;
	GLContext*				GLContext::m_currentGLContextBound = NULL;


	GLContext::GLContext( const OpenGLInfo& openGLInfo)
		:
	m_openGLInfo(openGLInfo)
	,m_handle((unsigned int)this)
	,m_rendererString(NULL)
	,m_vendorString(NULL)
	,m_versionString(NULL)
	,m_extensionsString(NULL)
	{
		m_contexts.push_back( this);
	}

	GLContext::~GLContext( void)
	{
		if( isThisTheCurrentGLContext())
			BREAKPOINT( GLContext is still bound while trying to be deleted!);

		// remove this from m_contexts
		bool found = false;
		unsigned int numContexts = m_contexts.size();
		unsigned int i = 0;
		for( ; i < numContexts; ++i)
			if( m_contexts[i] == this)
				found = true;

		if( found)
			m_contexts.erase( m_contexts.begin() + i - 1);
		else
			BREAKPOINT(GLContext was not registered);
	}

	
	void GLContext::Enable ( const GLenum param) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(GLEnable is called through context that is currently not bound!);

		glEnable(param);
	}
	
	void GLContext::Disable( const GLenum param) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(GLDisable is called through context that is currently not bound!);

		glDisable(param);
	}

	
	void GLContext::EnableDepthBuffer( void) const
	{
		Enable(GL_DEPTH_TEST);
	}

	void GLContext::DisableDepthBuffer( void) const
	{
		Disable(GL_DEPTH_TEST);
	}

	void GLContext::EnableDepthWrite( void) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(EnableDepthWrite is called through context that is currently not bound!);

		glDepthMask( true);
	}

	void GLContext::DisableDepthWrite( void) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(DisableDepthWrite is called through context that is currently not bound!);
		
		glDepthMask( false);
	}

	
	void GLContext::SetDepthFunc(GLenum depthFunc /* = GL_LESS */) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(DisableDepthWrite is called through context that is currently not bound!);
		
		glDepthFunc(depthFunc);
	}

	
	void GLContext::EnableCulling( void) const
	{
		Enable(GL_CULL_FACE);
	}

	void GLContext::DisableCulling( void) const
	{
		Disable(GL_CULL_FACE);
	}

	void GLContext::SetCullFace( const GLenum cullFace) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(SetCullFace is called through context that is currently not bound!);

		glCullFace(cullFace);
	}


	void GLContext::EnableAlphaBlending( void) const
	{
		Enable(GL_BLEND);
	}

	void GLContext::DisableAlphaBlending( void) const
	{
		Disable(GL_BLEND);
	}

	void GLContext::SetBlendFunc( const GLenum source, const GLenum dest) const
	{	
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(SetBlendFunc is called through context that is currently not bound!);

		glBlendFunc(source, dest);
	}


	void GLContext::ResizeViewport( const GLint x, const GLint y, 
									const GLsizei width, const GLsizei height) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(ResizeViewport is called through context that is currently not bound!);
		
		glViewport( x, y, width, height);
	}
	

	void GLContext::ClearBuffers( const GLulong color /*= 0x0*/, 
								  const GLbitfield buffersToClear /*= GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT*/) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(Clear is called through context that is currently not bound!);

		GLclampf r = (GLclampf)((color >> 24) & 0xff) / 255.0f;
		GLclampf g = (GLclampf)((color >> 16) & 0xff) / 255.0f;
		GLclampf b = (GLclampf)((color >> 8) & 0xff) / 255.0f;
		GLclampf a = (GLclampf)((color) && 0xff) / 255.0f;

		glClearColor( r, g, b, a);
		glClear( buffersToClear);
	}

	
	const char* const GLContext::GetRenderer( void) const
	{
		return m_rendererString;
	}

	const char* const GLContext::GetVendor( void) const
	{
		return m_vendorString;
	}

	const char* const GLContext::GetVersion( void) const
	{
		return m_versionString;
	}

	const char* const GLContext::GetExtensions( void) const
	{
		return m_extensionsString;
	}

	bool GLContext::HasGLError( void) const
	{
		if ( !isThisTheCurrentGLContext())
			BREAKPOINT(HasGLError is called through context that is currently not bound!);

#ifdef _DEBUG
		{
			GLenum error = glGetError();

			if( error == GL_NO_ERROR)
				return false;

			switch (error)
			{
			case GL_INVALID_ENUM:
				printf("OpenGL Invalid Enum \n");
				break;
			case GL_INVALID_VALUE:
				printf("OpenGL Invalid Value \n");
				break;
			case GL_INVALID_OPERATION:
				printf("OpenGL Invalid Operation \n");
				break;
			case GL_STACK_OVERFLOW:
				printf("OpenGL Stack Overflow \n");
				break;
			case GL_STACK_UNDERFLOW:
				printf("OpenGL Stack Underflow \n");
				break;
			case GL_OUT_OF_MEMORY:
				printf("OpenGL Out of Mem \n");
				break;
			}

			return true;
		}
#endif;
		return false;
	}


	/* --- Private functions --- */
	bool GLContext::isThisTheCurrentGLContext( void) const
	{
		return this == m_currentGLContextBound ? true : false;
	}

	
	GLContext*& GLContext::GetContext( const unsigned int handle)
	{
		for( unsigned int i = 0; i < m_contexts.size(); ++i)
		{
			if( (unsigned int)m_contexts[i] == handle)
				return m_contexts[i];
		}
	}


}