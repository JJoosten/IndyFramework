// Juul Joosten 2013

#include "Game.h"

#include "Ocean\Ocean.h"

#include "Paths.h"

#include <IndyCore/CoreDefines.h>
#include <IndyGL/Camera/Camera.h>
#include <IndyGL/Window/Window.h>
#include <IndyGL/Context/GLContext.h>
#include <IndyGL/Shader/GLSLShader.h>
#include <IndyGL/Shader/GLSLShaderProgram.h>
#include <IndyGL/Textures/Texture2D.h>
#include <IndyGL/Buffers/VertexAttributeBuffer.h>
#include <IndyGL/Buffers/UniformBuffer.h>
#include <IndyGL/Buffers/VertexArrayObject.h>
#include <IndyGL/Buffers/FrameBuffer.h>

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Indy
{
	class _imp_game : public GameBlueprint
	{
	public:
		_imp_game()
			:
		m_window(NULL),
		m_glContext(NULL),
		m_camera(NULL),
		m_frameBuffer(NULL)
		{
			
		}

		~_imp_game()
		{

		}

		void Create(GLContext* const openGLContext, Window* const window)
		{
			m_glContext = openGLContext;
			m_window = window;

			// setup viewport
			m_glContext->ResizeViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

			loadCamera();

			m_ocean = new Ocean();

			m_frameBuffer = new FrameBuffer();
			m_frameBuffer->Create(1280, 720, NumRenderTargets::ONE, FrameBufferDepth::DISABLED);

		}

		void Destroy(void)
		{
			delete m_frameBuffer;

			delete m_camera;

		}

		void UpdateFrame(const double deltaTimeSec)
		{
			m_ocean->UpdateFrame(deltaTimeSec);
		}

		void DrawFrame(const double deltaTimeSec)
		{
			// set clear color to black and clear depth and color buffer 
			m_glContext->ClearBuffers(0x0, ClearTargets::COLOR_AND_DEPTH_BUFFER);

			m_ocean->DrawFrame(*m_camera);

			glFinish();
		}

	private:
		void loadCamera(void)
		{
			float position[3] = {100,100,100};
			float target[3] = {0,0,0};
			float worldUp[3] = {0,1,0};

			m_camera = new Camera();
			m_camera->Init( 70.0f, (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 10000.0f);
			m_camera->LookAt( position, target, worldUp);
		}

	private:
		GLContext*				m_glContext;
		Window*					m_window;
		Camera*					m_camera;
		FrameBuffer*			m_frameBuffer;
		Ocean*					m_ocean;
	};
}