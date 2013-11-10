// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class GLSLShaderProgram;
	class UniformBuffer
	{

	public:
		UniformBuffer( void);
		~UniformBuffer( void);

		void Create( const GLSLShaderProgram* shaderProgram,
					 const GLchar* const uniformBufferName, 
					 const GLchar** const uniformSubNames, 
					 const GLshort numUniforms);

		void SetUniform( const GLuint index,
						 const GLvoid* data, 
						 const GLuint dataSizeInBytes);

		void SendToGPU( const GLuint blockIndex);

		void Bind( const GLuint blockIndex);
		void UnBind( void);

	private:
		GLubyte*	m_buffer;
		GLint*		m_offsets;
		GLint		m_sizeInBytes;
		GLuint		m_numUniforms;
		GLuint		m_handle;
	};
}