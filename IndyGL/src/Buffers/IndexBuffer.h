// Juul Joosten 2013

#pragma once

#include "DrawModeEnums.h"

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class IndexBuffer : public NonCopyable
	{

	public:
		IndexBuffer( void);
		~IndexBuffer( void);

		void Create( const GLuint numIndices, 
					 const GLuint sizeOfElementInBytes, 
					 const void* const indices, 
					 const BufferUsage::BufferUse usage = BufferUsage::STATIC_DRAW,
					 const DrawModes::DrawMode drawMode = DrawModes::TRIANGLES);

		void Destroy( void);

		void DestroyLocalBuffer( void);

		void Bind( void) const;
		void Unbind( void) const;


		GLuint GetID( void) const { return m_iboID; }

		GLuint GetNumIndices( void) const { return m_numIndices; }

		DrawModes::DrawMode GetDrawMode( void) const { return m_drawMode; }

		GLuint GetSizeOfElementInBytes( void) const { return m_sizeOfElementInBytes; }

		const GLvoid* const GetReadOnlyIndexData( void) const { return m_indices; }

	private:
		GLuint	m_iboID;
		GLuint	m_numIndices;
		GLuint  m_sizeOfElementInBytes;
		GLvoid* m_indices;
		DrawModes::DrawMode  m_drawMode;
	};
}