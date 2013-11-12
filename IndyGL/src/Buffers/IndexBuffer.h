// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class IndexBuffer
	{

	public:
		IndexBuffer( void);
		IndexBuffer( const IndexBuffer& cpyctr);
		~IndexBuffer( void);

		const IndexBuffer& operator=( const IndexBuffer& rvalue);

		void Create( const GLuint numIndices, void* const indices, const GLuint sizeOfElementInBytes, const GLenum usage = GL_STATIC_DRAW);
		void Destroy( void);

		void DestroyLocalBuffer( void);

		void Bind( void) const;
		void Unbind( void) const;


		GLuint GetID( void) const { return m_iboID; }

		GLuint GetNumIndices( void) const { return m_numIndices; }

		const GLvoid* const GetReadOnlyIndexData( void) const { return m_indices; }

	private:
		GLuint	m_iboID;
		GLuint	m_numIndices;
		GLuint  m_sizeOfElementInBytes;
		GLvoid* m_indices;
	};
}