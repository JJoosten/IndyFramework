#include "FrameBuffer.h"
#include "../Textures/Texture2D.h"
#include <IndyCore/CoreDefines.h>

namespace Indy
{
	FrameBuffer::FrameBuffer( void)
		:
	m_framebufferID(0)
	,m_width(0)
	,m_height(0)
	,m_numRTs(NumRenderTargets::ONE)
	,m_renderTargets(NULL)
	,m_depthBuffer(NULL)
	,m_mipMappingEnabled(FrameBufferMipMaps::DISABLED)
	,m_depthBufferEnabled(FrameBufferDepth::DISABLED)
	{

	}

	FrameBuffer::~FrameBuffer( void)
	{

	}

	
	void FrameBuffer::Create( const unsigned int width, 
							  const unsigned int height, 
							  const NumRenderTargets::NumRenderTargets numRTs /*= NumRenderTargets::ONE*/,
							  const FrameBufferDepth::FrameBufferDepthBuffer depthBufferEnabled /*= FrameBufferDepth::ENABLED*/, 
							  const FrameBufferMipMaps::FrameBufferMipMapping mipMappingEnabled /*= FrameBufferMipMaps::DISABLED*/)
	{
		m_width = width;
		m_height = height;
		m_numRTs = numRTs;
		m_mipMappingEnabled = mipMappingEnabled;
		m_depthBufferEnabled = depthBufferEnabled;

		
		glGenFramebuffers(1, &m_framebufferID);
		Bind();
		
		TextureCreateMipMaps::TextureCreateMipMap textureMipMappingEnabled = TextureCreateMipMaps::NONE;
		if( textureMipMappingEnabled == FrameBufferMipMaps::ENABLED)
			textureMipMappingEnabled = TextureCreateMipMaps::ON_GPU;

		// setup render targets
		m_renderTargets = new Texture2D[numRTs];
		for( int i = 0; i < m_numRTs; ++i)
		{
			m_renderTargets[i].Create( m_width, m_height, TextureFormats::RGBA, TextureInternalFormats::RGBA8, TextureStorageLocations::GPU_MEMORY, textureMipMappingEnabled);
			m_renderTargets[i].SetSamplerFilter(TextureMinFilters::LINEAR, TextureMagFilters::LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_renderTargets[i].GetTextureID(), 0);
		}

		if(m_depthBufferEnabled == FrameBufferDepth::ENABLED)
		{
			m_depthBuffer = new Texture2D();
			m_depthBuffer->Create( m_width, m_height, TextureFormats::DEPTH, TextureInternalFormats::DEPTH, TextureStorageLocations::GPU_MEMORY, textureMipMappingEnabled);
			m_depthBuffer->SetSamplerFilter(TextureMinFilters::LINEAR, TextureMagFilters::LINEAR);
			m_depthBuffer->SetTextureWrapMode( TextureWrapModes::CLAMP_TO_EDGE, TextureWrapModes::CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->GetTextureID(), 0);
		}

		Unbind();
	}

	void FrameBuffer::Destroy( void)
	{
		for( int i = 0; i < m_numRTs; ++i)
			m_renderTargets[i].Destroy();
		delete[] m_renderTargets;

		if(m_depthBufferEnabled == FrameBufferDepth::ENABLED)
		{
			m_depthBuffer->Destroy();
			delete m_depthBuffer;
		}

		glDeleteFramebuffers( 1, &m_framebufferID);
		m_framebufferID = 0;

		m_width = 0;
		m_height = 0;
		m_numRTs = NumRenderTargets::ONE;
		m_mipMappingEnabled = FrameBufferMipMaps::DISABLED;
		m_depthBufferEnabled = FrameBufferDepth::DISABLED;
	}


	void FrameBuffer::Bind( void) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	}

	void FrameBuffer::Unbind( void) const
	{
		if(m_depthBufferEnabled == FrameBufferDepth::ENABLED)
		{
			for( int i = 0; i < m_numRTs; ++i)
				m_renderTargets[i].UpdateMipMaps();

			if(m_depthBuffer != NULL)
				m_depthBuffer->UpdateMipMaps();
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::BindRTAsTexture( const RenderTargets::RenderTarget rtIndex /*= RenderTargets::RT0*/) const
	{
		m_renderTargets[rtIndex].Bind();
	}

	void FrameBuffer::UnbindRTAsTexture( void) const
	{
		glBindTexture( GL_TEXTURE_2D, 0);
	}

	void FrameBuffer::BindDepthBufferAsTexture( void) const
	{
		if(m_depthBufferEnabled == FrameBufferDepth::DISABLED)
			BREAKPOINT(This framebuffer has no depthbuffer);

		m_depthBuffer->Bind();
	}

	void FrameBuffer::UnbindDepthBufferAsTexture( void) const
	{
		if(m_depthBufferEnabled == FrameBufferDepth::DISABLED)
			BREAKPOINT(This framebuffer has no depthbuffer);

		m_depthBuffer->Unbind();
	}
}