#include "IndexBuffer.h"
#include <IndyCore/CoreDefines.h>

namespace Indy
{
	IndexBuffer::IndexBuffer( void)
		:
	m_iboID(0)
	,m_numIndices(0)
	,m_sizeOfElementInBytes(0)
	,m_indices(NULL)
	,m_drawMode(DrawModes::TRIANGLES)
	{

	}

	IndexBuffer::~IndexBuffer( void)
	{
		if(m_iboID != 0)
			BREAKPOINT( IndexBuffer was not yet destroyed);

		if(m_numIndices != 0)
			BREAKPOINT( IndexBuffer was not yet destroyed);
	}
	
	void IndexBuffer::Create( const GLuint numIndices, 
							  const GLuint sizeOfElementInBytes, 
							  const void* const indices, 
							  const BufferUsage::BufferUse usage /*= BufferUsage::STATIC_DRAW*/,
							  const DrawModes::DrawMode drawMode /*= DrawModes::TRIANGLES*/)
	{
		if(m_indices != 0)
			BREAKPOINT( IndexBuffer was already created);

		if(m_numIndices != 0)
			BREAKPOINT( IndexBuffer was already created);

		m_drawMode = drawMode;
		m_numIndices = numIndices;
		m_indices = new char[sizeOfElementInBytes * numIndices];
		memcpy(m_indices, indices, sizeOfElementInBytes * numIndices);

		m_sizeOfElementInBytes = sizeOfElementInBytes;

		glGenBuffers(1, &m_iboID);

		Bind();
		
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeOfElementInBytes * m_numIndices, m_indices, usage);

		Unbind();
	}

	void IndexBuffer::Destroy( void)
	{
		if(m_iboID == 0)
			BREAKPOINT( IndexBuffer was not yet created);

		glDeleteBuffers(1, &m_iboID);

		m_iboID = 0;
		m_numIndices = 0;
		m_sizeOfElementInBytes = 0;

		if(m_indices != NULL)
			DestroyLocalBuffer();
	}
	
	void IndexBuffer::DestroyLocalBuffer( void)
	{
		if(m_indices == NULL)
			BREAKPOINT( IndexBuffer m_indices is already deleted);

		delete[] m_indices;
		m_indices = NULL;
	}

	void IndexBuffer::Bind( void) const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	}

	void IndexBuffer::Unbind( void) const
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}