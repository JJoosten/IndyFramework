// Juul Joosten 2013

#pragma once

#include <vector>

#include <IndyCore/Utilities/NonCopyable.h>

#include "GL/glew.h"


namespace Indy
{
	struct OpenGLInfo
	{
		unsigned int MajorVersion;
		unsigned int MinorVersion;
	};

	class GLContext : public NonCopyable
	{

	public:
		GLContext ( const OpenGLInfo& info);
		~GLContext( void);
		
		virtual void Bind  ( void) = 0;
		virtual void Unbind( void) = 0;

		void Enable ( const GLenum param) const;
		void Disable( const GLenum param) const;

		void EnableDepthBuffer( void) const;
		void DisableDepthBuffer( void) const;
		void EnableDepthWrite( void) const;
		void DisableDepthWrite( void) const;

		void EnableCulling( void) const;
		void DisableCulling( void) const;
		void SetCullFace( const GLenum cullFace = GL_BACK) const;


		void EnableAlphaBlending( void) const;
		void DisableAlphaBlending( void) const;
		void SetBlendFunc( const GLenum source = GL_SRC_ALPHA, 
						   const GLenum dest = GL_ONE_MINUS_SRC_ALPHA) const;


		void ResizeViewport( const GLint x, const GLint y, 
							 const GLsizei width, const GLsizei height) const;

		void ClearBuffers( const GLulong color = 0x0, 
						   const GLbitfield buffersToClear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) const;

		bool HasGLError( void) const;


		/* --- Getters & Setters --- */
		const OpenGLInfo& GetOpenGLInfo( void) const { return m_openGLInfo; }

		static GLContext*& GetCurrentBoundGLContext( void) { return m_currentGLContextBound; }


	protected:
		bool isThisTheCurrentGLContext( void) const;


	protected:
		OpenGLInfo		m_openGLInfo;
		
		// static data
		static std::vector<GLContext*>	m_contexts;
		static GLContext*				m_currentGLContextBound;
	};
}