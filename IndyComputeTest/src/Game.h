// Juul Joosten 2013

#pragma once

#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class GLContext;
	class GLSLShader;
	class GLSLShaderProgram;
	class Texture2D;
	class VertexAttributeBuffer;
	class UniformBuffer;
	class VertexArray;
	class Game : public NonCopyable
	{

	public:
		Game( void);
		~Game( void);

		void UpdateFrame( const double deltaTimeSec);
		void DrawFrame  ( const double deltaTimeSec);


	private:
		void loadComputeShader( void);
		void loadVertexFragmentShader( void);
		void loadTexture( void);
		void loadPlaneVertexData( void);

	private:
		GLContext*&				m_glContext;
		GLSLShader*				m_computeShader;
		GLSLShaderProgram*		m_computeShaderProgram;
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_vertexFragShaderProgram;
		Texture2D*				m_texture;
		VertexAttributeBuffer*	m_planeVertices;
		UniformBuffer*			m_cameraDataUBO;
		VertexArray*			m_vertexArray;
	};
}