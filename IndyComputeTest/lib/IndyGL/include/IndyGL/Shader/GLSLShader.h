// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	namespace GLSLShaderTypes
	{
		enum GLSLShaderType
		{
			VERTEX_SHADER	= GL_VERTEX_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
			GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
			COMPUTE_SHADER	= GL_COMPUTE_SHADER
		};
	};

	class GLSLShader
	{
		friend class GLSLShaderProgram;

	public:
		GLSLShader( void);
		~GLSLShader( void);

		bool LoadSourceFromFile( const GLSLShaderTypes::GLSLShaderType shaderType, 
								 const char* const sourceFile);

		// ownership of source is transfered to shader
		bool LoadSource( const GLSLShaderTypes::GLSLShaderType shaderType, 
						 char* const source, const unsigned int shaderSourceSizeInBytes);

		void Create( void);

		bool Compile( void);

		/* --- Getters & Setters --- */
		unsigned int GetID( void) const { return m_shaderID; }

		const GLchar* const GetShaderSource( void) const { return m_shaderSource; }

		GLint GetShaderSourceSizeInBytes( void) const { return m_shaderSourceSizeInBytes; }


	private:
		// used to keep track of shaders in shader programs
		void addToReferenceCounter( void);
		void substractFromReferenceCounter( void);


	private:
		GLchar*							m_shaderSource;
		GLint							m_shaderSourceSizeInBytes;
		GLuint							m_shaderID;
		int								m_usageCounter;
		GLSLShaderTypes::GLSLShaderType m_shaderType;
	};
}