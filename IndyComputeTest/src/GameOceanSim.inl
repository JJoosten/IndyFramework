// Juul Joosten 2013

#include "Game.h"

#include "Ocean\Ocean.h"

#include "Paths.h"

#include <IndyCore/CoreDefines.h>
#include <IndyCore/Input/Keyboard.h>
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

		void Create(GLContext* const openGLContext, Window* const window, Keyboard* const keyboard)
		{
			m_glContext = openGLContext;
			m_window = window;
			m_keyboard = keyboard;

			// setup viewport
			m_glContext->ResizeViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

			loadCamera();

			m_ocean = new Ocean(64.0f, Vector2f(32.0f, 32.0f), 0.0005f);

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
			float speed = 10;
			float rSpeed = 20;

			// update camera
			if (m_keyboard->IsKeyDown(KeyboardKeys::W))
				m_camera->MoveOverRelativeZ(-speed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::S))
				m_camera->MoveOverRelativeZ(speed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::A))
				m_camera->MoveOverRelativeX(-speed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::D))
				m_camera->MoveOverRelativeX(speed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::UP))
				m_camera->Pitch(rSpeed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::DOWN))
				m_camera->Pitch(-rSpeed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::LEFT))
				m_camera->Yaw(-rSpeed * deltaTimeSec);
			if (m_keyboard->IsKeyDown(KeyboardKeys::RIGHT))
				m_camera->Yaw(rSpeed * deltaTimeSec);

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
			float pos[3] = { 10.0f, 45.0f, -30.0f };
			float pitch = 60;

			m_camera = new Camera();
			m_camera->Init( 70.0f, (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 10000.0f);
			m_camera->SetPosition(pos);
			m_camera->Pitch(pitch);
		}

	private:
		GLContext*				m_glContext;
		Window*					m_window;
		Camera*					m_camera;
		FrameBuffer*			m_frameBuffer;
		Ocean*					m_ocean;
		Keyboard*				m_keyboard;
	};
}