// Juul Joosten 2013

#include "GLSLShader.h"

#include "GLSLShaderPreprocessor.h"

namespace Indy
{
	GLSLShader::GLSLShader( void)
		:
	m_shaderID(0),
	m_shaderType(),
	m_shaderSource(NULL),
	m_shaderSourceSizeInBytes(0),
	m_usageCounter(0)
	{

	}

	GLSLShader::~GLSLShader( void)
	{
		if( m_usageCounter > 0)
			BREAKPOINT(~GLSLShader still being used by one or more shader programs);

		if(m_shaderID != 0)
			glDeleteShader( m_shaderID);

		if(m_shaderSource != NULL)
			delete m_shaderSource;
		m_shaderSource = NULL;
	}


	bool GLSLShader::LoadSourceFromFile( const GLSLShaderTypes::GLSLShaderType shaderType, 
										 const char* const sourceFile)
	{
		m_shaderType = shaderType;

		ShaderSourceData sourceData = GLSLShaderPreprocessor( sourceFile);

		if( sourceData.Source == NULL)
			return false;

		m_shaderSourceSizeInBytes = sourceData.SourceSizeInBytes;
		m_shaderSource = sourceData.Source;

		return true;
	}

	bool GLSLShader::LoadSource( const GLSLShaderTypes::GLSLShaderType shaderType, 
								 GLchar* const source, const GLuint shaderSourceSizeInBytes)
	{
		m_shaderType = shaderType;

		if( m_shaderSource)
			delete[] m_shaderSource;

		m_shaderSource = source;
		m_shaderSourceSizeInBytes = shaderSourceSizeInBytes;

		return true;
	}

	
	void GLSLShader::Create( void)
	{
		if( m_shaderID)
			BREAKPOINT( This shader is already created!);

		m_shaderID = glCreateShader(m_shaderType);
	}

	bool GLSLShader::Compile( void)
	{
		if(m_shaderID == 0)
			BREAKPOINT( Shader is not yet created!);

		glShaderSource( m_shaderID, 1, (const GLchar**)&m_shaderSource, &m_shaderSourceSizeInBytes);

		glCompileShader( m_shaderID);

		GLint result = 0xDEADBEEF;
		glGetShaderiv( m_shaderID, GL_COMPILE_STATUS, &result);

		if ( result != GL_TRUE)
		{
			GLsizei stringLength = 0;

			char infoLog[1024];
			memset( infoLog, 0, 1024);
			glGetShaderiv( m_shaderID, GL_INFO_LOG_LENGTH, &stringLength);
			glGetShaderInfoLog( m_shaderID, stringLength, 0, infoLog);
			printf("Shader Error Log: %s \n", infoLog);
		
			BREAKPOINT( Shader compile error occured);

			return false;
		}

		return true;
	}

	void GLSLShader::addToReferenceCounter( void)
	{
		++m_usageCounter;
	}

	void GLSLShader::substractFromReferenceCounter( void)
	{
		--m_usageCounter;

		if( m_usageCounter < 0)
			BREAKPOINT(Reference counter decremented more then there were references);
	}
}