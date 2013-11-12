// Juul Joosten 2013

#pragma once

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class VertexAttributeBuffer : public NonCopyable
	{

	public:
		VertexAttributeBuffer( void);

		~VertexAttributeBuffer( void);


		// ownership of attribute data is transferred
		void Create( const GLuint attributeBindLocation, 
					 const GLuint numAttributes, 
					 const GLuint numAttributeChannels, 
					 GLvoid* const attributeData, 
					 const GLuint attributeSizeInBytes,
					 const GLenum usage = GL_STATIC_DRAW );

		void Destroy( void);

		void ReSend( const GLuint numElementsToSend, 
					 const GLuint numElementsOffset = 0);


		void Bind( void) const;
		void Unbind( void) const;


		/* --- Getters & Setters --- */
		GLuint GetID( void) const { return m_vboID; }

		GLuint GetNumAttributes( void) const { return m_numAttributes; }

		GLuint GetNumAttributeChannels( void) const { return m_numAttributeChannels; }

		GLuint GetAttributeSizeInBytes( void) const { return m_attributeSizeInBytes; }

		const GLvoid* const GetReadOnlyAttributeData( void) const { return m_attributeData; }

	private:
		GLuint  m_vboID;
		GLuint	m_attributeBindLocation;
		GLuint	m_numAttributes;
		GLuint  m_numAttributeChannels;
		GLubyte m_attributeSizeInBytes;
		GLvoid*	m_attributeData;
	};
}