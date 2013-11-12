// Juul Joosten 2013

#include "VertexArrayObject.h"
#include "VertexAttributeBuffer.h"
#include "IndexBuffer.h"

#include <IndyCore/CoreDefines.h>

namespace Indy
{
	VertexArrayObject::VertexArrayObject( void)
		:
	m_handle(0)
	{
		glGenVertexArrays( 1, &m_handle);
	}

	VertexArrayObject::~VertexArrayObject( void)
	{
		glDeleteVertexArrays( 1, &m_handle);
	}

	void VertexArrayObject::Bind( void) const
	{
		glBindVertexArray( m_handle);
	}

	void VertexArrayObject::Unbind( void) const
	{
		glBindVertexArray( 0);
	}

	
	void VertexArrayObject::BindIndexBuffer( const IndexBuffer* const indexBuffer) const
	{
		this->Bind();
		indexBuffer->Bind();
		this->Unbind();
		indexBuffer->Unbind();
	}

	void VertexArrayObject::VertexAttributePointer( const VertexAttributeBuffer* const buffer,
												    const GLuint index, 
												    const GLint size, 
												    const GLenum type, 
												    const GLboolean normalized, 
												    const GLsizei stride) const
	{
		buffer->Bind();
		this->VertexAttributePointer(index, size, type, normalized, stride, NULL);
		buffer->Unbind();
	}

	void VertexArrayObject::VertexAttributePointer( const GLuint index, 
												    const GLint size, 
												    const GLenum type, 
												    const GLboolean normalized, 
												    const GLsizei stride, 
												    const GLvoid* pointer) const
	{
		this->Bind();
		
		glEnableVertexAttribArray(index);
		
		glVertexAttribPointer( index, size, type, normalized, stride, pointer);

		this->Unbind();

	}

	
	bool VertexArrayObject::IsVertexAttributeArrayEnabled( const GLuint index) const
	{
		this->Bind();

		GLuint isVertexAttribEnabled = GL_FALSE;
		glGetVertexAttribIuiv( index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isVertexAttribEnabled);

		this->Unbind();
		
		return isVertexAttribEnabled == GL_TRUE ? true : false;
	}
}