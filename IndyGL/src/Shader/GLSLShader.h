// Juul Joosten 2013

#pragma once

#include "GL/glew.h"
#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	namespace GLSLShaderTypes
	{
		enum GLSLShaderType
		{
			VERTEX_SHADER	              = GL_VERTEX_SHADER,
			FRAGMENT_SHADER               = GL_FRAGMENT_SHADER,
			GEOMETRY_SHADER               = GL_GEOMETRY_SHADER,
			TESSELATION_CONTROL_SHADER    = GL_TESS_CONTROL_SHADER,
			TESSELATION_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
			COMPUTE_SHADER	              = GL_COMPUTE_SHADER
		};
	};

	class GLSLShader : public NonCopyable
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

		void Destroy( void);

		bool Compile( void) const;

		/* --- Getters & Setters --- */
		unsigned int GetID( void) const { return m_shaderID; }

		const GLchar* const GetShaderSource( void) const { return m_shaderSource; }

		GLint GetShaderSourceSizeInBytes( void) const { return m_shaderSourceSizeInBytes; }

		static bool ComputeShaderSupportAvailable(void) { return GLEW_ARB_compute_shader; }

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