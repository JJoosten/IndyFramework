// Juul Joosten 2013

#include "VertexArray.h"

namespace Indy
{
	VertexArray::VertexArray( void)
		:
	m_handle(0)
	{
		glGenVertexArrays(1, &m_handle);
	}

	VertexArray::~VertexArray( void)
	{
		glDeleteVertexArrays( 1, &m_handle);
	}

	void VertexArray::Bind( void)
	{
		glBindVertexArray( m_handle);
	}

	void VertexArray::UnBind( void)
	{
		glBindVertexArray(0);
	}

	void VertexArray::VertexAttributePointer( GLuint index, 
											  GLint size, 
											  GLenum type, 
											  GLboolean normalized, 
											  GLsizei stride, 
											  GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	}
}