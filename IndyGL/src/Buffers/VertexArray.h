// Juul Joosten 2013

#pragma once

#include "GL/glew.h"
#include <vector>

#define MAX_VERTEX_ARRAY_ATTRIBUTES 32

namespace Indy
{
	class VertexAttributeBuffer;
	class VertexArray
	{
	public:
		VertexArray( void);
		~VertexArray( void);

		void Bind( void) const;
		void Unbind( void) const;

		void VertexAttributePointer( const VertexAttributeBuffer* const buffer,
									 const GLuint index, 
									 const GLint size, 
									 const GLenum type,
									 const GLboolean normalized,
									 const GLsizei stride) const;

		void VertexAttributePointer( const GLuint index, 
									 const GLint size, 
									 const GLenum type,
									 const GLboolean normalized,
									 const GLsizei stride, 
									 const GLvoid* pointer) const;

		bool IsVertexAttributeArrayEnabled( const GLuint index) const;

		GLuint GetHandle( void) const { return m_handle; }


	private:
		GLuint m_handle;
	};
}