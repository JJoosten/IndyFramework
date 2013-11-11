// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class VertexAttributeBuffer;
	class VertexArray
	{
	public:
		VertexArray( void);
		~VertexArray( void);

		void Bind( void) const;
		void UnBind( void) const;

		void VertexAttributePointer( const VertexAttributeBuffer* const buffer,
									 const GLuint index, 
									 const GLint size, 
									 const GLenum type,
									 const GLboolean normalized,
									 const GLsizei stride, 
									 const GLvoid* pointer);

		void VertexAttributePointer( const GLuint index, 
									 const GLint size, 
									 const GLenum type,
									 const GLboolean normalized,
									 const GLsizei stride, 
									 const GLvoid* pointer);

	private:
		GLuint m_handle;

	};
}