// Juul Joosten 2013

#include "GLSLShaderProgram.h"
#include "GLSLShader.h"

#include <IndyCore/CoreDefines.h>

namespace Indy
{

	GLSLShaderProgram::GLSLShaderProgram( void)
		:
	m_shaderProgramID(0),
	m_vertexShader(NULL),
	m_fragmentShader(NULL),
	m_geometryShader(NULL),
	m_computeShader(NULL)
	{

	}

	GLSLShaderProgram::~GLSLShaderProgram( void)
	{
		if( m_vertexShader != NULL)
		{
			glDetachShader( m_shaderProgramID, m_vertexShader->GetID());
			m_vertexShader->substractFromReferenceCounter();
		}

		if( m_fragmentShader != NULL)
		{
			glDetachShader( m_shaderProgramID, m_fragmentShader->GetID());
			m_fragmentShader->substractFromReferenceCounter();
		}

		if( m_geometryShader != NULL)
		{
			glDetachShader( m_shaderProgramID, m_geometryShader->GetID());
			m_geometryShader->substractFromReferenceCounter();
		}

		if( m_computeShader != NULL)
		{
			glDetachShader( m_shaderProgramID, m_computeShader->GetID());
			m_computeShader->substractFromReferenceCounter();
		}	

		if( m_shaderProgramID != 0)
			glDeleteProgram( m_shaderProgramID);
	}

	
	void GLSLShaderProgram::SetVertexShader  ( GLSLShader* const vertexShader)
	{
		if( m_vertexShader != NULL)
			m_vertexShader->substractFromReferenceCounter();

		m_vertexShader = vertexShader;
		m_vertexShader->addToReferenceCounter();
	}

	void GLSLShaderProgram::SetFragmentShader( GLSLShader* const fragmentShader)
	{
		if( m_fragmentShader != NULL)
			m_fragmentShader->substractFromReferenceCounter();

		m_fragmentShader = fragmentShader;
		m_fragmentShader->addToReferenceCounter();
	}

	void GLSLShaderProgram::SetGeometryShader( GLSLShader* const geometryShader)
	{
		if( m_geometryShader != NULL)
			m_geometryShader->substractFromReferenceCounter();

		m_geometryShader = geometryShader;
		m_geometryShader->addToReferenceCounter();
	}

	void GLSLShaderProgram::SetComputeShader ( GLSLShader* const computeShader)
	{
		if( m_computeShader != NULL)
			m_computeShader->substractFromReferenceCounter();

		m_computeShader = computeShader;
		m_computeShader->addToReferenceCounter();
	}


	void GLSLShaderProgram::Create( void)
	{
		m_shaderProgramID = glCreateProgram();
	}

	bool GLSLShaderProgram::Link( void)
	{
		if( m_vertexShader != NULL)
			glAttachShader( m_shaderProgramID, m_vertexShader->GetID());

		if( m_fragmentShader != NULL)
			glAttachShader( m_shaderProgramID, m_fragmentShader->GetID());

		if( m_geometryShader != NULL)
			glAttachShader( m_shaderProgramID, m_geometryShader->GetID());

		if( m_computeShader != NULL)
			glAttachShader( m_shaderProgramID, m_computeShader->GetID());

		glLinkProgram( m_shaderProgramID);
		
		GLint result = 0xDEADBEEF;
		glGetProgramiv( m_shaderProgramID, GL_LINK_STATUS, &result);

		if ( result != GL_TRUE)
		{
			GLsizei stringLength = 0;

			char infoLog[1024];
			memset( infoLog, 0, 1024);
			glGetProgramInfoLog( m_shaderProgramID, stringLength, 0, infoLog);
			printf("Shader Error Log: %s \n", infoLog);
		
			BREAKPOINT( Shader link error occured);

			return false;
		}


		return true;
	}


	
	void GLSLShaderProgram::Bind( void)
	{
		if( m_shaderProgramID == 0)
			BREAKPOINT(Shader Program is not yet created);

		glUseProgram( m_shaderProgramID);
	}

	void GLSLShaderProgram::UnBind( void)
	{
		glUseProgram(NULL);
	}

	
	GLuint GLSLShaderProgram::GetUniformBlockIndex( const GLchar* uniformBlockName) const
	{
		GLuint location = glGetUniformBlockIndex( m_shaderProgramID, uniformBlockName); 
		return location;
	}
	
	GLint GLSLShaderProgram::GetUniformLocation( const GLchar* uniform) const
	{
		return glGetUniformLocation( m_shaderProgramID, uniform);
	}
	

	void GLSLShaderProgram::SetUniformf( const GLchar* uniform, const GLfloat value) const
	{
		SetUniformf( GetUniformLocation(uniform), value);
	}

	void GLSLShaderProgram::SetUniformf( const GLint location, const GLfloat value) const
	{
		glUniform1f( location, value);
	}

	
	void GLSLShaderProgram::SetUniform3f( const GLchar* uniform, const GLfloat* value) const
	{
		SetUniform3f( GetUniformLocation( uniform), value);
	}

	void GLSLShaderProgram::SetUniform3f( const GLint location, const GLfloat* value) const
	{
		glUniform3fv( location, 1, value);
	}
	

	void GLSLShaderProgram::SetUniform4x4f( const GLchar* uniform, const GLfloat* value, const GLboolean transpose /* = GL_FALSE */) const
	{
		SetUniform4x4f( GetUniformLocation(uniform), value, transpose);
	}

	void GLSLShaderProgram::SetUniform4x4f( const GLint location, const GLfloat* value, const GLboolean transpose /* = GL_FALSE */) const
	{
		glUniformMatrix4fv( location, 1, 0, value);
	}


	void GLSLShaderProgram::SetUniformi( const GLchar* uniform, const GLint value) const
	{
		SetUniformi( GetUniformLocation(uniform), value);
	}

	void GLSLShaderProgram::SetUniformi( const GLint location, const GLint value) const
	{
		glUniform1i( location, value);
	}

	
	void GLSLShaderProgram::SetUniform2i( const GLchar* uniform, const GLint* value) const
	{
		SetUniform2i(  GetUniformLocation(uniform), value);
	}

	void GLSLShaderProgram::SetUniform2i( const GLint location, const GLint* value) const
	{
		glUniform2iv( location, 1, value);
	}
}
