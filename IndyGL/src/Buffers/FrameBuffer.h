// Juul Joosten 2013

#pragma once

#include "FrameBufferEnums.h"

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
					 const NumRenderTargets::NumRenderTargets numRTs = NumRenderTargets::ONE,
					 const FrameBufferDepth::FrameBufferDepthBuffer depthBufferEnabled = FrameBufferDepth::ENABLED, 
					 const FrameBufferMipMaps::FrameBufferMipMapping mipMappingEnabled = FrameBufferMipMaps::DISABLED);
		void Destroy( void);


		void Bind( void) const;
		void Unbind( void) const;

		void BindRTAsTexture( const RenderTargets::RenderTarget rtIndex = RenderTargets::RT0) const;
		void UnbindRTAsTexture( void) const;

		void BindDepthBufferAsTexture( void) const;
		void UnbindDepthBufferAsTexture( void) const;

		GLuint GetID( void) const { return m_framebufferID; }

		unsigned int GetWidth( void) const { return m_width; }

		unsigned int GetHeight( void) const { return m_height; }

		unsigned int GetNumRenderTargets( void) const { return m_numRTs; }

		bool IsUsingMipMaps( void) const { return (bool)m_mipMappingEnabled; }

		bool HasDepthBuffer( void) const { return (bool)m_depthBufferEnabled; }

	private:
		GLuint								m_framebufferID;
		Texture2D*							m_renderTargets;
		Texture2D*							m_depthBuffer;
		unsigned int						m_width;
		unsigned int						m_height;
		NumRenderTargets::NumRenderTargets			m_numRTs;
		FrameBufferDepth::FrameBufferDepthBuffer	m_depthBufferEnabled;
		FrameBufferMipMaps::FrameBufferMipMapping	m_mipMappingEnabled;
	};
}