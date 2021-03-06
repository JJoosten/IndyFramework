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
		m_computeShader(NULL),
		m_computeShaderProgram(NULL),
		m_texture(NULL),
		m_vertexShader(NULL),
		m_fragmentShader(NULL),
		m_vertexFragShaderProgram(NULL),
		m_planeVertices(NULL),
		m_cameraDataUBO(NULL),
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

			loadComputeShader();

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

			m_computeShaderProgram->Destroy();
			delete m_computeShaderProgram;

			m_computeShader->Destroy();
			delete m_computeShader;

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

			m_cameraDataUBO->Destroy();
			delete m_cameraDataUBO;

			delete m_vertexArrayObject;
		}

		void UpdateFrame(const double deltaTimeSec)
		{

			if (GLEW_ARB_compute_shader)
			{
				glBindImageTexture(0, m_texture->GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

				m_computeShaderProgram->Bind();

				glDispatchCompute(m_window->GetWidth() / 8, m_window->GetHeight() / 8, 1);

				m_computeShaderProgram->Unbind();

				glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R8);
			}
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
		void loadComputeShader(void)
		{
			// load compute shader and create compute shader program
			m_computeShader = new GLSLShader();
			m_computeShader->LoadSourceFromFile(GLSLShaderTypes::COMPUTE_SHADER, CONTENT_SHADER_PATH "raytracer.comp");
			m_computeShader->Create();
			if (!m_computeShader->Compile())
				BREAKPOINT(Compiling compute shader failed!);

			m_computeShaderProgram = new GLSLShaderProgram();
			m_computeShaderProgram->SetComputeShader(m_computeShader);
			m_computeShaderProgram->Create();
			if (!m_computeShaderProgram->Link())
				BREAKPOINT(Linking compute shader program failed!);

			enum CameraDataUniform
			{
			VIEWPLANE_BL,
			VIEWPLANE_DELTA_XY,
			END
			};

			// compute camera info
			const float halfWidth = (float)m_window->GetWidth() * 0.5f;
			const float halfHeight = (float)m_window->GetHeight() * 0.5f;

			const GLfloat fov = 70.0f;
			const float focalLength = 2.0f / tan( fov / 2.0f);
			const float aspectRatio = halfWidth / halfHeight;
			const glm::vec3 viewPlaneBL = glm::vec3( -1.0f * aspectRatio, -1.0f, focalLength);
			glm::vec3 dxy = glm::abs(viewPlaneBL) / glm::vec3( halfWidth, halfHeight, 1.0f);
			dxy.z = 0;

			// compute dxy including aspect ratio compenstation
			const GLchar* uniformBufferSubNames[] = {  "ViewPlaneBL", "ViewPlaneDeltaXY" } ;
			m_cameraDataUBO = new UniformBuffer();
			m_cameraDataUBO->Create(m_computeShaderProgram, "CameraInfo", (const GLchar** const )uniformBufferSubNames, END);
			m_cameraDataUBO->SetUniform(VIEWPLANE_BL, (GLvoid*)&viewPlaneBL[0], sizeof(GLfloat) * 3);
			m_cameraDataUBO->SetUniform(VIEWPLANE_DELTA_XY, (GLvoid*)&dxy[0], sizeof(GLfloat) * 3);
			m_cameraDataUBO->SendToGPU( m_computeShaderProgram->GetUniformBlockIndex("CameraInfo"));
		}
		
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
			// load texture to adjust in compute shader
			m_texture = new Texture2D();
			m_texture->Create(m_window->GetWidth(), m_window->GetHeight(), TextureFormats::RGBA, TextureInternalFormats::RGBA32F, TextureStorageLocations::GPU_MEMORY, TextureCreateMipMaps::ON_GPU);
			m_texture->SetSamplerFilter(TextureMinFilters::LINEAR, TextureMagFilters::LINEAR);

			m_computeShaderProgram->SetUniformi("Texture", 0);

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
			m_vertexArrayObject->VertexAttributePointer(m_planeVertices, 0, 2, GL_FLOAT, NormalizeFixedPointValues::FALSE, 0);
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
		GLSLShader*				m_computeShader;
		GLSLShaderProgram*		m_computeShaderProgram;
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_vertexFragShaderProgram;
		Texture2D*				m_texture;
		VertexAttributeBuffer*	m_planeVertices;
		UniformBuffer*			m_cameraDataUBO;
		VertexArrayObject*		m_vertexArrayObject;
		FrameBuffer*			m_frameBuffer;
	};
}