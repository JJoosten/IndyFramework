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
	,m_numRTs(0)
	,m_renderTargets(NULL)
	,m_depthBuffer(NULL)
	,m_isUsingMipMaps(false)
	,m_hasDepthBuffer(false)
	{

	}

	FrameBuffer::~FrameBuffer( void)
	{

	}

	
	void FrameBuffer::Create( const unsigned int width, 
							  const unsigned int height, 
							  const unsigned int numRTs		/* = 1 */,
							  const bool useDepthBuffer     /* = true */,
							  const bool useMipMaps			/* = false */)
	{
		m_width = width;
		m_height = height;
		m_isUsingMipMaps = useMipMaps;
		m_numRTs = numRTs;
		m_hasDepthBuffer = useDepthBuffer;

		
		glGenFramebuffers(1, &m_framebufferID);
		Bind();
		
		// setup render targets
		m_renderTargets = new Texture2D[numRTs];
		for( unsigned int i = 0; i < m_numRTs; ++i)
		{
			m_renderTargets[i].Create( m_width, m_height, NULL, 4, 1);
			m_renderTargets[i].GenerateGPUTexture(m_isUsingMipMaps);
			m_renderTargets[i].Bind();
			m_renderTargets[i].SetSamplerFilter(GL_LINEAR, GL_LINEAR);
			m_renderTargets[i].Unbind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_renderTargets[i].GetTextureID(), 0);
		}

		if(m_hasDepthBuffer)
		{
			m_depthBuffer = new Texture2D();
			m_depthBuffer->Create( m_width, m_height, NULL, 1, 3); // 24 bit depth texture
			m_depthBuffer->GenerateGPUTexture(false);
			m_depthBuffer->Bind();
			m_depthBuffer->SetSamplerFilter(GL_LINEAR, GL_LINEAR);
			m_depthBuffer->SetTextureWrapMode( GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			m_depthBuffer->Unbind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer->GetTextureID(), 0);
		}

		Unbind();
	}

	void FrameBuffer::Destroy( void)
	{
		for( unsigned int i = 0; i < m_numRTs; ++i)
			m_renderTargets[i].Destroy();
		delete[] m_renderTargets;

		if(m_hasDepthBuffer)
		{
			m_depthBuffer->Destroy();
			delete m_depthBuffer;
		}

		glDeleteFramebuffers( 1, &m_framebufferID);
		m_framebufferID = 0;

		m_width = 0;
		m_height = 0;
		m_numRTs = 0;
		m_isUsingMipMaps = false;
		m_hasDepthBuffer = false;
	}


	void FrameBuffer::Bind( void) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	}

	void FrameBuffer::Unbind( void) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::BindRTAsTexture( const unsigned int rtIndex /* = 0 */) const
	{
		if(rtIndex >= m_numRTs)
			BREAKPOINT(rtIndex is bigger then numRenderTargets);

		m_renderTargets[rtIndex].Bind();

	}

	void FrameBuffer::UnbindRTAsTexture( const unsigned int rtIndex /* = 0 */) const
	{
		if(rtIndex >= m_numRTs)
			BREAKPOINT(rtIndex is bigger then numRenderTargets);

		m_renderTargets[rtIndex].Unbind();
	}

	void FrameBuffer::BindDepthBufferAsTexture( void) const
	{
		if(!m_hasDepthBuffer)
			BREAKPOINT(This framebuffer has no depthbuffer);

		m_depthBuffer->Bind();
	}

	void FrameBuffer::UnbindDepthBufferAsTexture( void) const
	{
		if(!m_hasDepthBuffer)
			BREAKPOINT(This framebuffer has no depthbuffer);

		m_depthBuffer->Unbind();
	}
}