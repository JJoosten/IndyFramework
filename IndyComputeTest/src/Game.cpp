// Juul Joosten 2013

#include "Game.h"

#include <IndyGL/Context/GLContext.h>
#include <IndyGL/Shader/GLSLShader.h>
#include <IndyGL/Shader/GLSLShaderProgram.h>
#include <IndyGL/Textures/Texture2D.h>
#include <IndyGL/Buffers/VertexAttributeBuffer.h>
#include <IndyGL/Buffers/UniformBuffer.h>
#include <IndyGL/Buffers/VertexArray.h>

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Indy
{
	Game::Game( void)
		:
	m_glContext( GLContext::GetCurrentBoundGLContext()),
	m_computeShader( NULL),
	m_computeShaderProgram( NULL),
	m_texture( NULL),
	m_vertexShader( NULL),
	m_fragmentShader( NULL),
	m_vertexFragShaderProgram( NULL),
	m_planeVertices( NULL),
	m_cameraDataUBO( NULL),
	m_vertexArray( NULL)
	{
		// setup viewport
		m_glContext->ResizeViewport( 0, 0, 1280, 720);

		loadComputeShader();

		loadVertexFragmentShader();

		loadTexture();

		loadPlaneVertexData();
	}

	Game::~Game( void)
	{
		delete m_computeShaderProgram;
		delete m_computeShader;
		delete m_vertexFragShaderProgram;
		delete m_vertexShader;
		delete m_fragmentShader;
		delete m_texture;
		delete m_planeVertices;
		delete m_cameraDataUBO;
		delete m_vertexArray;
	}

	void Game::UpdateFrame( const double deltaTimeSec)
	{
		if(GLEW_ARB_compute_shader)
		{
			m_cameraDataUBO->Bind( m_computeShaderProgram->GetUniformBlockIndex("CameraInfo"));

			glBindImageTexture( 0, m_texture->GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);  

			m_computeShaderProgram->Bind();

			glDispatchCompute( 1280 / 8, 720 / 8, 1);
			
			m_computeShaderProgram->UnBind();
			
			glBindImageTexture( 0, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R8);

			m_cameraDataUBO->UnBind();
		}
	}

	void Game::DrawFrame( const double deltaTimeSec)
	{
		// set clear color to black and clear depth and color buffer 
		m_glContext->ClearBuffers( 0x0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_texture->Bind();

		m_vertexFragShaderProgram->Bind();
		
		m_vertexArray->Bind();

		glDrawArrays( GL_TRIANGLES, 0, 6);
		
		m_vertexArray->UnBind();

		m_vertexFragShaderProgram->UnBind();

		m_texture->UnBind();

		glFinish();
	}


	/* --- private ---*/
	void Game::loadComputeShader( void)
	{
		// load compute shader and create compute shader program
		m_computeShader = new GLSLShader();
		m_computeShader->LoadSourceFromFile( GLSLShaderTypes::COMPUTE_SHADER, "content/shaders/raytracer.comp");
		m_computeShader->Create();
		if( !m_computeShader->Compile())
			printf("Compiling compute shader failed!");

		m_computeShaderProgram = new GLSLShaderProgram();
		m_computeShaderProgram->SetComputeShader(m_computeShader);
		m_computeShaderProgram->Create();
		if( !m_computeShaderProgram->Link())
			printf("Linking compute shader program failed!");


		enum CameraDataUniform
		{
			VIEWPLANE_BL,
			VIEWPLANE_DELTA_XY,
			END
		};

		// compute camera info
		const float halfWidth = 640.0f;
		const float halfHeight = 360.0f;

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

	void Game::loadVertexFragmentShader( void)
	{
		// load default vertex and fragment shader
		m_vertexShader = new GLSLShader();
		m_vertexShader->LoadSourceFromFile( GLSLShaderTypes::VERTEX_SHADER, "content/shaders/planeVertexShader.vert");
		m_vertexShader->Create();
		if( !m_vertexShader->Compile())
			printf("Compiling vertex shader failed!");
		
		m_fragmentShader = new GLSLShader();
		m_fragmentShader->LoadSourceFromFile( GLSLShaderTypes::FRAGMENT_SHADER, "content/shaders/defaultFragmentShader.frag");
		m_fragmentShader->Create();
		if( !m_fragmentShader->Compile())
			printf("Compiling fragment shader failed!");

		m_vertexFragShaderProgram = new GLSLShaderProgram();
		m_vertexFragShaderProgram->SetVertexShader(m_vertexShader);
		m_vertexFragShaderProgram->SetFragmentShader(m_fragmentShader);
		m_vertexFragShaderProgram->Create();
		if( !m_vertexFragShaderProgram->Link())
			printf("Linking vertex frag shader program failed!");
	}

	void Game::loadTexture( void)
	{
		// enable textures
		m_glContext->Enable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);

		// load texture to adjust in compute shader
		float* texData = new float[1280 * 720 * 4];
		memset( texData, 0, 1280 * 720 * 4);
		m_texture = new Texture2D( 1280, 720, (unsigned char*)texData, 4, sizeof(float));
		m_texture->CreateGPUTexture(false);
		m_texture->DeleteLocalData();
		m_texture->Bind();
		m_texture->SetSamplerFilter(GL_NEAREST, GL_NEAREST);
		m_texture->UnBind();
		delete texData;
		
		m_computeShaderProgram->SetUniformi( "Texture", 0);

		m_vertexFragShaderProgram->SetUniformi( "Texture", m_texture->GetTextureID());
	}

	void Game::loadPlaneVertexData( void)
	{
		// create vertices
		m_planeVertices = new VertexAttributeBuffer();
		float* vertices = new float[6 * 2];
		vertices[ 0] = -1; vertices[ 1] = -1; // TL 
		vertices[ 2] =  1; vertices[ 3] = -1; // TR
		vertices[ 4] =  1; vertices[ 5] =  1; // BL

		vertices[ 6] = -1; vertices[ 7] = -1; // BL
		vertices[ 8] =  1; vertices[ 9] =  1; // TR
		vertices[10] = -1; vertices[11] =  1; // BR
		m_planeVertices->Create(0, 6, 2, vertices, sizeof(GLfloat));

		// generate vertex arrays
		m_vertexArray = new VertexArray();
		m_vertexArray->Bind();
			
		m_planeVertices->Bind();
		
		// index 0, size of 2 (2 attributes xy), type float, not normalized, 0 offset and 0 data due to binding of m_planeVertices
		m_vertexArray->VertexAttributePointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		
		m_planeVertices->UnBind();

		m_vertexArray->UnBind();
	}
}