// Juul Joosten 2013

#pragma once

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class Texture2D;
	class FrameBuffer : public NonCopyable
	{

	public:
		FrameBuffer( void);
		~FrameBuffer( void);

		void Create( const unsigned int width, 
					 const unsigned int height, 
					 const unsigned int numRTs = 1,
					 const bool useDepthBuffer = true, 
					 const bool useMipMaps = false);
		void Destroy( void);


		void Bind( void) const;
		void Unbind( void) const;

		void BindRTAsTexture( const unsigned int rtIndex = 0) const;
		void UnbindRTAsTexture( const unsigned int rtIndex = 0) const;

		void BindDepthBufferAsTexture( void) const;
		void UnbindDepthBufferAsTexture( void) const;

		GLuint GetID( void) const { return m_framebufferID; }

		unsigned int GetWidth( void) const { return m_width; }

		unsigned int GetHeight( void) const { return m_height; }

		unsigned int GetNumRenderTargets( void) const { return m_numRTs; }

		bool IsUsingMipMaps( void) const { return m_isUsingMipMaps; }

		bool HasDepthBuffer( void) const { return m_hasDepthBuffer; }

	private:
		GLuint			m_framebufferID;
		Texture2D*		m_renderTargets;
		Texture2D*		m_depthBuffer;
		unsigned int	m_width;
		unsigned int	m_height;
		unsigned int	m_numRTs;
		bool			m_isUsingMipMaps;
		bool			m_hasDepthBuffer;
	};
}