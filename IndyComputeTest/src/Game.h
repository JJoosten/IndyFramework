// Juul Joosten 2013

#pragma once

#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class Window;
	class GLContext;
	class Camera;
	class GLSLShader;
	class GLSLShaderProgram;
	class Texture2D;
	class VertexAttributeBuffer;
	class UniformBuffer;
	class VertexArrayObject;
	class Game : public NonCopyable
	{

	public:
		Game( GLContext* const openGLContext, Window* const window);
		~Game( void);

		void UpdateFrame( const double deltaTimeSec);
		void DrawFrame  ( const double deltaTimeSec);


	private:
		void loadComputeShader( void);
		void loadVertexFragmentShader( void);
		void loadTexture( void);
		void loadPlaneVertexData( void);
		void loadCamera( void);

	private:
		GLContext*				m_glContext;
		Window*					m_window;
		Camera*					m_camera;
		GLSLShader*				m_computeShader;
		GLSLShaderProgram*		m_computeShaderProgram;
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_vertexFragShaderProgram;
		Texture2D*				m_texture;
		VertexAttributeBuffer*	m_planeVertices;
		UniformBuffer*			m_cameraDataUBO;
		VertexArrayObject*		m_vertexArrayObject;
	};
}