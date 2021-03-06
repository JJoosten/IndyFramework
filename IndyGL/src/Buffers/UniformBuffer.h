// Juul Joosten 2013

#pragma once

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class GLSLShaderProgram;
	class UniformBuffer : public NonCopyable
	{

	public:
		UniformBuffer( void);
		~UniformBuffer( void);

		void Create( const GLSLShaderProgram* shaderProgram,
					 const GLchar* const uniformBufferName, 
					 const GLchar** const uniformSubNames, 
					 const GLshort numUniforms);

		void Destroy( void);

		void SetUniform( const GLuint index,
						 const GLvoid* data, 
						 const GLuint dataSizeInBytes);

		void SendToGPU( const GLuint blockIndex) const;

		void Bind( const GLuint blockIndex) const;
		void Unbind( void) const;

	private:
		GLint		m_sizeInBytes;
		GLuint		m_numUniforms;
		GLuint		m_handle;
		GLubyte*	m_buffer;
		GLint*		m_offsets;
	};
}