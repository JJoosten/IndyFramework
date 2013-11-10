// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class VertexAttributeBuffer
	{

	public:
		VertexAttributeBuffer( void);

		~VertexAttributeBuffer( void);


		// ownership of attribute data is transferred
		void Create( GLuint attributeBindLocation, 
					 GLuint numAttributes, 
					 GLuint numAttributeChannels, 
					 GLvoid* attributeData, 
					 GLuint attributeSizeInBytes,
					 GLenum usage = GL_STATIC_DRAW );

		void ReSend( const GLuint numElementsToSend, 
					 const GLuint numElementsOffset = 0);


		void Bind( void);
		void UnBind( void);


		/* --- Getters & Setters --- */
		GLuint GetID( void) const { return m_vboID; }

		GLuint GetNumAttributes( void) const { return m_numAttributes; }

		GLuint GetNumAttributeChannels( void) const { return m_numAttributeChannels; }



	private:
		GLvoid*	m_attributeData;
		GLuint  m_vboID;
		GLuint	m_attributeBindLocation;
		GLuint	m_numAttributes;
		GLuint  m_numAttributeChannels;
		GLubyte m_attributeSizeInBytes;
	};
}