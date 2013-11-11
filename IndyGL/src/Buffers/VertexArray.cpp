// Juul Joosten 2013

#include "VertexArray.h"
#include "VertexAttributeBuffer.h"

#include <IndyCore/CoreDefines.h>

namespace Indy
{
	VertexArray::VertexArray( void)
		:
	m_handle(0)
	,m_vertexAttributePointerIndexFlags(0)
	{
		glGenVertexArrays( 1, &m_handle);
	}

	VertexArray::~VertexArray( void)
	{
		glDeleteVertexArrays( 1, &m_handle);
		m_vertexAttributePointerIndexFlags = 0;
	}

	void VertexArray::Bind( void) const
	{
		glBindVertexArray( m_handle);

		for(unsigned int i = 0; i < 32; ++i)
		{
			if((1 << i) & m_vertexAttributePointerIndexFlags)
				glEnableVertexAttribArray(i);
		}
	}

	void VertexArray::UnBind( void) const
	{
		for(unsigned int i = 0; i < 32; ++i)
		{
			if((1 << i) & m_vertexAttributePointerIndexFlags)
				glDisableVertexAttribArray(i);
		}

		glBindVertexArray( 0);
	}

	void VertexArray::VertexAttributePointer( const VertexAttributeBuffer* const buffer,
											  const GLuint index, 
											  const GLint size, 
											  const GLenum type, 
											  const GLboolean normalized, 
											  const GLsizei stride)
	{
		buffer->Bind();
		this->VertexAttributePointer(index, size, type, normalized, stride, NULL);
		buffer->UnBind();
	}

	void VertexArray::VertexAttributePointer( const GLuint index, 
											  const GLint size, 
											  const GLenum type, 
											  const GLboolean normalized, 
											  const GLsizei stride, 
											  const GLvoid* pointer)
	{
		m_vertexAttributePointerIndexFlags |= 1 << index;

		Bind();
		
		glVertexAttribPointer( index, size, type, normalized, stride, pointer);

		UnBind();
	}

	
	bool VertexArray::IsVertexAttributeArrayEnabled( const GLuint index) const
	{
		Bind();

		GLuint isVertexAttribEnabled = GL_FALSE;
		glGetVertexAttribIuiv( index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isVertexAttribEnabled);

		UnBind();
		
		return isVertexAttribEnabled == GL_TRUE ? true : false;
	}
}