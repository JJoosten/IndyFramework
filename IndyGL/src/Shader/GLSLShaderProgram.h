// Juul Joosten 2013

#pragma once

#include "GL/glew.h"

namespace Indy
{
	class GLSLShader;
	class GLSLShaderProgram
	{

	public:
		GLSLShaderProgram( void);
		~GLSLShaderProgram( void);

		void SetVertexShader  ( GLSLShader* const vertexShader);
		void SetFragmentShader( GLSLShader* const fragmentShader);
		void SetGeometryShader( GLSLShader* const geometryShader);
		void SetComputeShader ( GLSLShader* const computeShader);

		
		void Create( void);

		bool Link( void);


		void Bind( void);
		void UnBind( void);

		GLuint GetUniformBlockIndex( const GLchar* uniformBlockName) const;

		GLint GetUniformLocation( const GLchar* uniform) const;
		
		void SetUniformf( const GLchar* uniform, const GLfloat value) const;
		void SetUniformf( const GLint location, const GLfloat value) const;

		void SetUniform3f( const GLchar* uniform, const GLfloat* value) const;
		void SetUniform3f( const GLint location, const GLfloat* value) const;

		void SetUniform4x4f( const GLchar* uniform, const GLfloat* value, const GLboolean transpose = GL_FALSE) const;
		void SetUniform4x4f( const GLint location, const GLfloat* value, const GLboolean transpose = GL_FALSE) const;

		void SetUniformi( const GLchar* uniform, const GLint value) const;
		void SetUniformi( const GLint location, const GLint value) const;
		
		void SetUniform2i( const GLchar* uniform, const GLint* value) const;
		void SetUniform2i( const GLint location, const GLint* value) const;


		/* --- Getters & Setters --- */
		GLuint GetID( void) const { return m_shaderProgramID; }


	private:
		GLuint			m_shaderProgramID;
		GLSLShader*		m_vertexShader;
		GLSLShader*		m_fragmentShader;
		GLSLShader*		m_geometryShader;
		GLSLShader*		m_computeShader;

	};
}