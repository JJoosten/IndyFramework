// Juul Joosten 2013

#include "Game.h"

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
		m_texture(NULL),
		m_vertexShader(NULL),
		m_fragmentShader(NULL),
		m_vertexFragShaderProgram(NULL),
		m_planeVertices(NULL),
		m_vertexArrayObject(NULL),
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

			loadVertexFragmentShader();

			loadTexture();

			loadPlaneVertexData();

			loadCamera();

			m_frameBuffer = new FrameBuffer();
			m_frameBuffer->Create(1280, 720, NumRenderTargets::ONE, FrameBufferDepth::DISABLED);

		}

		void Destroy(void)
		{
			delete m_frameBuffer;

			delete m_camera;

			m_vertexFragShaderProgram->Destroy();
			delete m_vertexFragShaderProgram;

			m_vertexShader->Destroy();
			delete m_vertexShader;

			m_fragmentShader->Destroy();
			delete m_fragmentShader;

			m_texture->Destroy();
			delete m_texture;

			m_planeVertices->Destroy();
			delete m_planeVertices;


			delete m_vertexArrayObject;
		}

		void UpdateFrame(const double deltaTimeSec)
		{

		}

		void DrawFrame(const double deltaTimeSec)
		{
			// set clear color to black and clear depth and color buffer 
			m_glContext->ClearBuffers(0x0, ClearTargets::COLOR_AND_DEPTH_BUFFER);


			// draw in frame buffer
			m_frameBuffer->Bind();

			m_texture->Bind();

			m_vertexFragShaderProgram->Bind();

			m_vertexArrayObject->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_vertexArrayObject->Unbind();

			m_vertexFragShaderProgram->Unbind();

			m_texture->Unbind();

			m_frameBuffer->Unbind();


			// draw framebuffer on quad
			m_frameBuffer->BindRTAsTexture();

			m_vertexFragShaderProgram->Bind();

			m_vertexArrayObject->Bind();

			glDrawArrays(GL_TRIANGLES, 0, 6);

			m_vertexArrayObject->Unbind();

			m_vertexFragShaderProgram->Unbind();

			m_frameBuffer->UnbindRTAsTexture();


			glFinish();
		}

	private:
		
		void loadVertexFragmentShader(void)
		{
			// load default vertex and fragment shader
			m_vertexShader = new GLSLShader();
			m_vertexShader->LoadSourceFromFile(GLSLShaderTypes::VERTEX_SHADER, CONTENT_SHADER_PATH "planeVertexShader.vert");
			m_vertexShader->Create();
			if (!m_vertexShader->Compile())
				BREAKPOINT(Compiling vertex shader failed!);

			m_fragmentShader = new GLSLShader();
			m_fragmentShader->LoadSourceFromFile(GLSLShaderTypes::FRAGMENT_SHADER, CONTENT_SHADER_PATH "defaultFragmentShader.frag");
			m_fragmentShader->Create();
			if (!m_fragmentShader->Compile())
				BREAKPOINT(Compiling fragment shader failed!);

			m_vertexFragShaderProgram = new GLSLShaderProgram();
			m_vertexFragShaderProgram->SetVertexShader(m_vertexShader);
			m_vertexFragShaderProgram->SetFragmentShader(m_fragmentShader);
			m_vertexFragShaderProgram->Create();
			if (!m_vertexFragShaderProgram->Link())
				BREAKPOINT(Linking vertex frag shader program failed!);
		}

		void loadTexture(void)
		{
			const unsigned int numChannels = 4;
			unsigned char* const textureData = new unsigned char[m_window->GetWidth() * m_window->GetHeight() * numChannels];
			for (unsigned int y = 0; y < m_window->GetHeight(); ++y)
			{
				for (unsigned int x = 0; x < m_window->GetWidth(); ++x)
				{
					unsigned int index = y * m_window->GetWidth() * numChannels + x * numChannels;
					
					textureData[index + 0] = x & 15;
					textureData[index + 1] = x & 15;
					textureData[index + 2] = x & 15;
					textureData[index + 3] = x & 15;
				}
			}

			// load texture to adjust in compute shader
			m_texture = new Texture2D();
			m_texture->Create(m_window->GetWidth(), m_window->GetHeight(), textureData, TextureFormats::RGBA, TextureInternalFormats::RGBA8UI, TextureStorageLocations::GPU_MEMORY, TextureCreateMipMaps::ON_GPU);
			m_texture->SetSamplerFilter(TextureMinFilters::LINEAR, TextureMagFilters::LINEAR);

			m_vertexFragShaderProgram->SetUniformi("Texture", m_texture->GetTextureID());
		}

		void loadPlaneVertexData(void)
		{
			// create vertices
			m_planeVertices = new VertexAttributeBuffer();
			float* vertices = new float[6 * 2];
			vertices[0] = -1; vertices[1] = -1; // TL 
			vertices[2] = 1; vertices[3] = -1; // TR
			vertices[4] = 1; vertices[5] = 1; // BL

			vertices[6] = -1; vertices[7] = -1; // BL
			vertices[8] = 1; vertices[9] = 1; // TR
			vertices[10] = -1; vertices[11] = 1; // BR
			m_planeVertices->Create(0, 6, 2, vertices, sizeof(GLfloat));

			// generate vertex arrays
			m_vertexArrayObject = new VertexArrayObject();

			// index 0, size of 2 (2 attributes xy), type float, not normalized, 0 offset and 0 data due to binding of m_planeVertices
			m_vertexArrayObject->VertexAttributePointer(m_planeVertices, 0, 2, GL_FLOAT, NormalizeFixedPointValues::TRUE, 0);
		}
		
		void loadCamera(void)
		{
			m_camera = new Camera();
			m_camera->Init(70.0f, (float)m_window->GetWidth() / (float)m_window->GetHeight(), 0.1f, 100.0f);
		}

	private:
		GLContext*				m_glContext;
		Window*					m_window;
		Camera*					m_camera;
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_vertexFragShaderProgram;
		Texture2D*				m_texture;
		VertexAttributeBuffer*	m_planeVertices;
		VertexArrayObject*		m_vertexArrayObject;
		FrameBuffer*			m_frameBuffer;
	};
}