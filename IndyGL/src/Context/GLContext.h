// Juul Joosten 2013

#pragma once

#include "GLContextEnums.h"

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
		
		unsigned int Create( void);
		void Destroy( void);

		virtual void Bind  ( void) = 0;
		virtual void Unbind( void) = 0;

		void Enable ( const GLenum param) const;
		void Disable( const GLenum param) const;

		void EnableDepthBuffer( void) const;
		void DisableDepthBuffer( void) const;
		void EnableDepthWrite( void) const;
		void DisableDepthWrite( void) const;
		void SetDepthFunc( const DepthFunc::DepthFunc depthFunc = DepthFunc::LESS) const;

		void EnableCulling( void) const;
		void DisableCulling( void) const;
		void SetCullFace( const CullFace::CullFace cullFace = CullFace::BACK) const;


		void EnableAlphaBlending( void) const;
		void DisableAlphaBlending( void) const;
		void SetBlendFunc( const BlendFunc::BlendFunc source = BlendFunc::SOURCE_ALPHA, 
						   const BlendFunc::BlendFunc destination = BlendFunc::ONE_MINUS_SOURCE_ALPHA) const;


		void ResizeViewport( const GLint x, const GLint y, 
							 const GLsizei width, const GLsizei height) const;

		void ClearBuffers( const GLulong color = 0x0, 
						   const ClearTargets::ClearTarget clearTargets = ClearTargets::COLOR_AND_DEPTH_BUFFER) const;

		const char* const GetRenderer( void) const;
		const char* const GetVendor( void) const;
		const char* const GetVersion( void) const;
		const char* const GetExtensions( void) const;


		bool HasGLError( void) const;


		/* --- Getters & Setters --- */
		const OpenGLInfo& GetOpenGLInfo( void) const { return m_openGLInfo; }

		unsigned int GetHandle( void) const { return m_handle; }

		static GLContext*& GetCurrentBoundGLContext( void) { return m_currentGLContextBound; }

		static GLContext*& GetContext( const unsigned int handle);


	protected:
		bool isThisTheCurrentGLContext( void) const;


	protected:
		OpenGLInfo		 m_openGLInfo;
		unsigned int	m_handle;
		
		char* m_rendererString;
		char* m_vendorString;
		char* m_versionString;
		char* m_extensionsString;

		// static data
		static std::vector<GLContext*>	m_contexts;
		static GLContext*				m_currentGLContextBound;
	};
}