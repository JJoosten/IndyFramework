// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class VertexArray
	{
	public:
		VertexArray( void);
		~VertexArray( void);

		void Bind( void);
		void UnBind( void);

		void VertexAttributePointer( GLuint index, 
									 GLint size, 
									 GLenum type,
									 GLboolean normalized,
									 GLsizei stride, 
									 GLvoid* pointer);

	private:
		GLuint m_handle;

	};
}