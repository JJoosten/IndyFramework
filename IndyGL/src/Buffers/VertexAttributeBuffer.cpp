// Juul Joosten 2013

#include "VertexAttributeBuffer.h"

#include <IndyCore/CoreDefines.h>

namespace Indy
{
	VertexAttributeBuffer::VertexAttributeBuffer( void)
			:
	m_vboID(NULL),
	m_attributeData(NULL),
	m_attributeBindLocation(0),
	m_numAttributes(0),
	m_attributeSizeInBytes(0)
	{

	}

	VertexAttributeBuffer::~VertexAttributeBuffer( void)
	{
		if( m_attributeData != 0)
			BREAKPOINT(VertexAttributeBuffer was not yet destroyed);
		
		if( m_vboID != 0)
			BREAKPOINT(VertexAttributeBuffer was not yet destroyed);
	}

	
	void VertexAttributeBuffer::Create( const GLuint attributeBindLocation, 
										const GLuint numAttributes, 
										const GLuint numAttributeChannels, 
										GLvoid* const attributeData, 
										const GLuint attributeSizeInBytes,
										const GLenum usage /* = GL_STATIC_DRAW */)
	{
		if(m_vboID != 0)
			BREAKPOINT(This AttributeBuffer is already in use!);

		m_attributeBindLocation = attributeBindLocation;
		m_numAttributes = numAttributes;
		m_numAttributeChannels = numAttributeChannels;
		m_attributeData = attributeData;
		m_attributeSizeInBytes = attributeSizeInBytes;

		if( m_numAttributes == 0)
			BREAKPOINT(numAttributes is zero);

		if( m_numAttributeChannels == 0 || m_numAttributeChannels > 32)
			BREAKPOINT(numAttributeChannels is not supported);

		if( m_attributeData == NULL)
			BREAKPOINT(AttributeData is NULL);

		glGenBuffers( 1, &m_vboID);

		Bind();
			
		glBufferData( GL_ARRAY_BUFFER, m_attributeSizeInBytes * m_numAttributes * m_numAttributeChannels, m_attributeData, usage);

		Unbind();
	}
	
	void VertexAttributeBuffer::Destroy( void)
	{
		if(m_attributeData == 0)
			BREAKPOINT(VertexAttributeBuffer was already destroyed or not initialized);
		
		if(m_vboID == 0)
			BREAKPOINT(VertexAttributeBuffer was already destroyed or not initialized);
		
		delete m_attributeData;
		m_attributeData = NULL;

		glDeleteBuffers( 1, &m_vboID);
		m_vboID = 0;
	}

	
	void VertexAttributeBuffer::ReSend( const GLuint numElementsToSend, 
									    const GLuint numElementsOffset /*= 0*/)
	{
		if( numElementsToSend > m_numAttributes ||
			numElementsToSend == 0)
			BREAKPOINT( numElementsToSend is bigger then original data or 0);

		if( numElementsOffset > m_numAttributes)
			BREAKPOINT( numElementsOffset is bigger then original data);

		Bind();

		glBufferSubData( GL_ARRAY_BUFFER, m_attributeSizeInBytes * numElementsOffset * m_numAttributeChannels, m_attributeSizeInBytes * numElementsToSend * m_numAttributeChannels, m_attributeData);

		Unbind();
	}

	void VertexAttributeBuffer::Bind( void) const
	{
		if(m_vboID == 0)
			BREAKPOINT(VertexAttributeBuffer is not yet a VBO);
			
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

	}

	void VertexAttributeBuffer::Unbind( void) const
	{
		if(m_vboID == 0)
			BREAKPOINT(VertexAttributeBuffer is not yet a VBO);
			
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}