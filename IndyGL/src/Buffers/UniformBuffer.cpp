// Juul Joosten 2013

#include "UniformBuffer.h"
#include "../Shader/GLSLShaderProgram.h"

#include <IndyCore/CoreDefines.h>

namespace Indy
{
	UniformBuffer::UniformBuffer( void)
		:
	m_buffer(NULL),
	m_offsets(NULL),
	m_sizeInBytes(0),
	m_numUniforms(0),
	m_handle(0)
	{

	}

	UniformBuffer::~UniformBuffer( void)
	{
		if( m_handle != 0)
			glDeleteBuffers( 1, &m_handle);

		if( m_buffer != NULL)
			delete m_buffer;
		m_buffer = NULL;

		if( m_offsets != NULL)
			delete m_offsets;
		m_offsets = NULL;
	}

	
	void UniformBuffer::Create( const GLSLShaderProgram* shaderProgram,
								const GLchar* const uniformBufferName, 
								const GLchar** const uniformSubNames, 
								const GLshort numUniforms)
	{
		m_numUniforms = numUniforms;

		glGetActiveUniformBlockiv( shaderProgram->GetID(), shaderProgram->GetUniformBlockIndex(uniformBufferName), GL_UNIFORM_BLOCK_DATA_SIZE, &m_sizeInBytes);
	
		if(m_sizeInBytes == 0)
			BREAKPOINT(UniformBuffer Create UniformBuffer has no size);

		m_buffer = new GLubyte[m_sizeInBytes];

		// get offsets per uniform
		GLuint* indices = new GLuint[m_numUniforms];
		glGetUniformIndices( shaderProgram->GetID(), m_numUniforms, uniformSubNames, indices);

		// validate the indices
		for( GLuint i = 0; i < m_numUniforms; ++i)
		{
			if(indices[i] == GLuint(0 - 1))
				BREAKPOINT(UniformBuffer Create not all the uniforms are used in the shader and thus excist);
		}

		m_offsets = new GLint[m_numUniforms];
		glGetActiveUniformsiv( shaderProgram->GetID(), m_numUniforms, indices, GL_UNIFORM_OFFSET, m_offsets);

		delete indices;
		
		glGenBuffers( 1, &m_handle);
	}

	void UniformBuffer::SetUniform( const GLuint index, 
									const GLvoid* data, 
									const GLuint dataSizeInBytes)
	{
		if( m_handle == 0 || m_buffer == NULL)
			BREAKPOINT( UniformBuffer is not yet created!);

		if( index >= m_numUniforms)
			BREAKPOINT( UniformBuffer SetUniform index is bigger then ammount of uniforms);

		const GLuint endOfMemWrite = m_offsets[index] + dataSizeInBytes;
		GLuint endOfBufferSegment = m_sizeInBytes;
		if(GLint(m_numUniforms) - 1 - GLint(index) > 0)
			endOfBufferSegment = m_offsets[index + 1];

		if(endOfMemWrite > endOfBufferSegment)
			BREAKPOINT(UniformBuffer SetUniform memcpy overflow);

		memcpy( m_buffer + m_offsets[index], data, dataSizeInBytes);
	}

	void UniformBuffer::SendToGPU( const GLuint blockIndex)
	{
		Bind( blockIndex);
		
		glBufferData( GL_UNIFORM_BUFFER, m_sizeInBytes, m_buffer, GL_DYNAMIC_DRAW );

		UnBind();
	}

	void UniformBuffer::Bind( const GLuint blockIndex)
	{
		glBindBuffer( GL_UNIFORM_BUFFER, m_handle);
		glBindBufferBase( GL_UNIFORM_BUFFER, blockIndex, m_handle );
	}

	void UniformBuffer::UnBind( void)
	{
		glBindBufferBase( GL_UNIFORM_BUFFER, 0, m_handle );
		glBindBuffer( GL_UNIFORM_BUFFER, 0);
	}
}