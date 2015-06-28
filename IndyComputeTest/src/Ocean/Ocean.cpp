#include "Ocean.h"
#include "../Paths.h"

#include <IndyCore\CoreDefines.h>
#include <IndyGL\Camera\Camera.h>
#include <IndyGL\Shader\GLSLShader.h>
#include <IndyGL\Shader\GLSLShaderProgram.h>
#include <IndyGL\Buffers\VertexArrayObject.h>
#include <IndyGL\Buffers\StorageBuffer.h>
#include <IndyGL\Geometry\VertexData.h>

Indy::Ocean::Ocean()
	:
m_vertices(NULL)
,m_vao(NULL)
,m_computeVertexTransform(NULL)
,m_computeVertexTransformProgram(NULL)
,m_vertexShader(NULL)
,m_fragmentShader(NULL)
,m_shadingProgram(NULL)
{
	createOceanMesh();

	initializeShaders();
}

Indy::Ocean::~Ocean()
{
	m_computeVertexTransform->Destroy();
	delete m_computeVertexTransform;

	m_computeVertexTransformProgram->Destroy();
	delete m_computeVertexTransformProgram;

	m_vertexShader->Destroy();
	delete m_vertexShader;

	m_fragmentShader->Destroy();
	delete m_fragmentShader;

	m_shadingProgram->Destroy();
	delete m_shadingProgram;

	delete m_vao;

	m_vertices->Destroy();
	delete m_vertices;

}

void Indy::Ocean::UpdateFrame(const double deltaTimeSec)
{
	if (Indy::GLSLShader::ComputeShaderSupportAvailable())
	{
		m_computeVertexTransformProgram->Bind();

		// send the delta time to ensure frame sync
		m_computeVertexTransformProgram->SetUniformf("DeltaTimeSec", deltaTimeSec);

		m_vertices->Bind();

		unsigned int workGroupSize = 32;

		// calculate num vertices to transform
		unsigned int arraySize = m_vertices->GetArraySize();

		// distribute work in quads
		int sqr = (int)ceil(std::sqrtf((float)arraySize));

		glDispatchCompute(sqr / workGroupSize, sqr / workGroupSize, 1);

		m_vertices->Unbind();

		m_computeVertexTransformProgram->Unbind();
	}
}

void Indy::Ocean::DrawFrame(const Indy::Camera& camera)
{
	m_shadingProgram->Bind();

	m_shadingProgram->SetUniform4x4f("ViewMat", camera.GetViewMatrix(), false);
	m_shadingProgram->SetUniform4x4f("ProjMat", camera.GetProjectionMatrix(), false);

	m_vao->Bind();

	glPointSize(10.0f);

	glDrawArrays(GL_POINTS, 0, m_vertices->GetArraySize());

	m_vao->Unbind();

	m_shadingProgram->Unbind();
}

void Indy::Ocean::initializeShaders()
{
	// load compute shader and create compute shader program
	m_computeVertexTransform = new GLSLShader();
	m_computeVertexTransform->LoadSourceFromFile(GLSLShaderTypes::COMPUTE_SHADER, CONTENT_SHADER_PATH "ocean.comp");
	m_computeVertexTransform->Create();
	if (!m_computeVertexTransform->Compile())
		BREAKPOINT(Compiling compute shader failed!);

	m_computeVertexTransformProgram = new GLSLShaderProgram();
	m_computeVertexTransformProgram->SetComputeShader(m_computeVertexTransform);
	m_computeVertexTransformProgram->Create();
	if (!m_computeVertexTransformProgram->Link())
		BREAKPOINT(Linking compute shader program failed!);


	// load default vertex and fragment shader
	m_vertexShader = new GLSLShader();
	m_vertexShader->LoadSourceFromFile(GLSLShaderTypes::VERTEX_SHADER, CONTENT_SHADER_PATH "ocean.vert");
	m_vertexShader->Create();
	if (!m_vertexShader->Compile())
		BREAKPOINT(Compiling vertex shader failed!);

	m_fragmentShader = new GLSLShader();
	m_fragmentShader->LoadSourceFromFile(GLSLShaderTypes::FRAGMENT_SHADER, CONTENT_SHADER_PATH "ocean.frag");
	m_fragmentShader->Create();
	if (!m_fragmentShader->Compile())
		BREAKPOINT(Compiling fragment shader failed!);

	m_shadingProgram = new GLSLShaderProgram();
	m_shadingProgram->SetVertexShader(m_vertexShader);
	m_shadingProgram->SetFragmentShader(m_fragmentShader);
	m_shadingProgram->Create();
	if (!m_shadingProgram->Link())
		BREAKPOINT(Linking vertex frag shader program failed!);
}

void Indy::Ocean::createOceanMesh()
{
	// create vao
	m_vao = new VertexArrayObject();

	const unsigned int numVerticesOnEdge = 64;
	const unsigned int numVertices = numVerticesOnEdge * numVerticesOnEdge;
	OceanVertex vertices[numVertices]; // test 64*64 vertices
	
	// spread the vertices
	for (unsigned int y = 0; y < numVerticesOnEdge; ++y)
	{
		for (unsigned int x = 0; x < numVerticesOnEdge; ++x)
		{
			const unsigned int index = y * numVerticesOnEdge + x;
			vertices[index].Pos = Vector3f((float)x * 10.0f - (float)numVerticesOnEdge * 5.0f, 0, (float)y * 10.0f - (float)numVerticesOnEdge * 5.0f);
		}
	}

	m_vertices = new StorageBuffer();
	m_vertices->Create(vertices, sizeof(OceanVertex), numVertices);

	char* offsetInBytes = (char*)sizeof(OceanVertex);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertices->GetID());
	m_vao->VertexAttributePointer(0, 3, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), 0);
	m_vao->VertexAttributePointer(1, 3, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), offsetInBytes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vertices->SetShaderLayoutIndex(0);
}
