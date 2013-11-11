// Juul Joosten 2013

#include "VertexArray.h"
#include "VertexAttributeBuffer.h"

namespace Indy
{
	VertexArray::VertexArray( void)
		:
	m_handle(0)
	{
		glGenVertexArrays( 1, &m_handle);
	}

	VertexArray::~VertexArray( void)
	{
		glDeleteVertexArrays( 1, &m_handle);
	}

	void VertexArray::Bind( void) const
	{
		glBindVertexArray( m_handle);
	}

	void VertexArray::UnBind( void) const
	{
		glBindVertexArray( 0);
	}

	void VertexArray::VertexAttributePointer( const VertexAttributeBuffer* const buffer,
											  const GLuint index, 
											  const GLint size, 
											  const GLenum type, 
											  const GLboolean normalized, 
											  const GLsizei stride, 
											  const GLvoid* pointer)
	{
		buffer->Bind();
		this->VertexAttributePointer(index, size, type, normalized, stride, pointer);
		buffer->UnBind();
	}

	void VertexArray::VertexAttributePointer( const GLuint index, 
											  const GLint size, 
											  const GLenum type, 
											  const GLboolean normalized, 
											  const GLsizei stride, 
											  const GLvoid* pointer)
	{
		Bind();
		glEnableVertexAttribArray( index);
		glVertexAttribPointer( index, size, type, normalized, stride, pointer);
		UnBind();
	}
}