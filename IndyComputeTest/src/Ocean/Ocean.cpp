#include "Ocean.h"
#include "../Paths.h"

#include <time.h>

#include <IndyCore\CoreDefines.h>
#include <IndyGL\Camera\Camera.h>
#include <IndyGL\Shader\GLSLShader.h>
#include <IndyGL\Shader\GLSLShaderProgram.h>
#include <IndyGL\Buffers\IndexBuffer.h>
#include <IndyGL\Buffers\VertexArrayObject.h>
#include <IndyGL\Buffers\StorageBuffer.h>
#include <IndyGL\Geometry\VertexData.h>


#include "../../content/SharedDefines.defines"

Indy::Ocean::Ocean(float length, Indy::Vector2f windVec, float phillipsSpectrum )
	:
m_vertices(NULL)
,m_randomValues(NULL)
,m_vao(NULL)
,m_computeVertexTransform(NULL)
,m_computeVertexTransformProgram(NULL)
,m_vertexShader(NULL)
,m_fragmentShader(NULL)
,m_shadingProgram(NULL)
, m_length(LENGTH)
, m_windVec(WIND_VEC)
, m_phillipsSpectrum(PHILLIPS)
, m_elapsedTimeInSec(0)
{
	initializeShaders();
	
	createOceanMesh();

	createRandomVariables();

	createhValues();

	srand(time(NULL));
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

	m_computeVertexInitialize->Destroy();
	delete m_computeVertexInitialize;

	m_computeVertexInitializeProgram->Destroy();
	delete m_computeVertexInitializeProgram;

	delete m_vao;

	m_vertices->Destroy();
	delete m_vertices;

	m_randomValues->Destroy();
	delete m_randomValues;

	m_hValues->Destroy();
	delete m_hValues;

	m_indexBuffer->Destroy();
	delete m_indexBuffer;

	m_dispersion->Destroy();
	delete m_dispersion;
}

void Indy::Ocean::UpdateFrame(const double deltaTimeSec)
{
	if (Indy::GLSLShader::ComputeShaderSupportAvailable())
	{
		m_computeVertexTransformProgram->Bind();

		// send the delta time to ensure frame sync
		m_computeVertexTransformProgram->SetUniformf("TimeSec", m_elapsedTimeInSec);
		m_computeVertexTransformProgram->SetUniformi("N", NUM_VERTICES_PER_EDGE);
		m_computeVertexTransformProgram->SetUniformf("G", 9.81f);
		m_computeVertexTransformProgram->SetUniformf("Length", m_length);

		m_vertices->Bind();

		m_hValues->Bind();

		m_dispersion->Bind();

		// distribute work in quads
		int numCompute = (NUM_VERTICES_PER_EDGE + (WORK_GROUP_SIZE / 2)) / WORK_GROUP_SIZE;
		glDispatchCompute(numCompute, numCompute, 1);

		m_dispersion->Unbind();

		m_hValues->Unbind();

		m_vertices->Unbind();

		m_computeVertexTransformProgram->Unbind();
	}

	m_elapsedTimeInSec += deltaTimeSec;
}

void Indy::Ocean::DrawFrame(const Indy::Camera& camera)
{
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	m_shadingProgram->Bind();

	m_shadingProgram->SetUniform4x4f("ViewMat", camera.GetViewMatrix(), false);
	m_shadingProgram->SetUniform4x4f("ProjMat", camera.GetProjectionMatrix(), false);

	m_vao->Bind();

	glEnable(GL_PRIMITIVE_RESTART);

	glPrimitiveRestartIndex( NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE);

	glDrawElements(m_indexBuffer->GetDrawMode(), m_indexBuffer->GetNumIndices(), GL_UNSIGNED_INT, 0);

	glDisable(GL_PRIMITIVE_RESTART);

	m_vao->Unbind();

	m_shadingProgram->Unbind();
}

void Indy::Ocean::initializeShaders()
{
	// load compute shaders and create compute shader program
	m_computeVertexTransform = new GLSLShader();
	m_computeVertexTransform->LoadSourceFromFile(GLSLShaderTypes::COMPUTE_SHADER, CONTENT_SHADER_PATH "oceanOptimized.comp");
	m_computeVertexTransform->Create();
	if (!m_computeVertexTransform->Compile())
		BREAKPOINT(Compiling compute shader failed!);

	m_computeVertexTransformProgram = new GLSLShaderProgram();
	m_computeVertexTransformProgram->SetComputeShader(m_computeVertexTransform);
	m_computeVertexTransformProgram->Create();
	if (!m_computeVertexTransformProgram->Link())
		BREAKPOINT(Linking compute shader program failed!);

	m_computeVertexInitialize = new GLSLShader();
	m_computeVertexInitialize->LoadSourceFromFile(GLSLShaderTypes::COMPUTE_SHADER, CONTENT_SHADER_PATH "oceanSetup.comp");
	m_computeVertexInitialize->Create();
	if (!m_computeVertexInitialize->Compile())
		BREAKPOINT(Compiling compute shader failed!);

	m_computeVertexInitializeProgram = new GLSLShaderProgram();
	m_computeVertexInitializeProgram->SetComputeShader(m_computeVertexInitialize);
	m_computeVertexInitializeProgram->Create();
	if (!m_computeVertexInitializeProgram->Link())
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

	const unsigned int numVertices = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	OceanVertex* vertices = new OceanVertex[numVertices]; // test 64*64 vertices

	// spread the vertices
	for (unsigned int y = 0; y < NUM_VERTICES_PER_EDGE; ++y)
	{
		float origZ = ((float)y - (float)NUM_VERTICES_PER_EDGE / 2.0f) * m_length / (float)NUM_VERTICES_PER_EDGE;

		for (unsigned int x = 0; x < NUM_VERTICES_PER_EDGE; ++x)
		{
			float origX = ((float)x - (float)NUM_VERTICES_PER_EDGE / 2.0f) * m_length / (float)NUM_VERTICES_PER_EDGE;

			const unsigned int index = y * NUM_VERTICES_PER_EDGE + x;

			vertices[index].Pos = Vector3f(origX, 0, origZ);
			vertices[index].OrigX = origX;
			vertices[index].Norm = Vector3f(0, 1, 0);
			vertices[index].OrigZ = origZ;
		}
	}

	m_vertices = new StorageBuffer();
	m_vertices->Create(vertices, sizeof(OceanVertex), numVertices);
	m_vertices->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_VERTICES);
	delete[] vertices;

	// indices for triangle strip

#if 1
	unsigned int numIndices = (NUM_VERTICES_PER_EDGE * 2 + 1) * (NUM_VERTICES_PER_EDGE - 1) - 1;
	unsigned int* indices = new unsigned int[numIndices];
	
	unsigned int checkValue = NUM_VERTICES_PER_EDGE * 2;
	unsigned int indexChecker = 0;
	unsigned int columnCounter = 0;
	unsigned int rowCounter = NUM_VERTICES_PER_EDGE;
	unsigned int i = 0;
	for (i; i < numIndices; ++i)
	{
		if (indexChecker != 0 && i % checkValue == 0)
		{
			indices[i] = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
			checkValue += NUM_VERTICES_PER_EDGE * 2 + 1;
		}
		else
		{
			indices[i] = indexChecker & 1 ? rowCounter++ : columnCounter++;
			indexChecker++;
		}
	}
	m_indexBuffer = new IndexBuffer();
	m_indexBuffer->Create(numIndices, 4, indices, BufferUsage::STATIC_DRAW, DrawModes::TRIANGLE_STRIP);
#else
	// generate triangles
	unsigned int numIndices = ((NUM_VERTICES_PER_EDGE - 1) * (NUM_VERTICES_PER_EDGE - 1) * 6);
	unsigned int* indices = new unsigned int[numIndices];
	for (unsigned int y = 0; y < NUM_VERTICES_PER_EDGE - 1; ++y)
	{
		for (unsigned int x = 0; x < NUM_VERTICES_PER_EDGE - 1; ++x)
		{
			unsigned int index = y * (NUM_VERTICES_PER_EDGE - 1) * 6 + x * 6;
			unsigned int vIndex = y * NUM_VERTICES_PER_EDGE + x;

			indices[index + 0] = vIndex;
			indices[index + 1] = vIndex + NUM_VERTICES_PER_EDGE;
			indices[index + 2] = vIndex + 1;
			indices[index + 5] = vIndex + 1;
			indices[index + 3] = vIndex + NUM_VERTICES_PER_EDGE;
			indices[index + 4] = vIndex + NUM_VERTICES_PER_EDGE + 1;
		}
	}

	m_indexBuffer = new IndexBuffer();
	m_indexBuffer->Create(numIndices, 4, indices, BufferUsage::STATIC_DRAW, DrawModes::TRIANGLES);

#endif

	char* offsetInBytes = (char*)(sizeof(Vector2f) * 2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices->GetID());
	m_vao->VertexAttributePointer(0, 4, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), 0);
	m_vao->VertexAttributePointer(4, 4, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), offsetInBytes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_vao->BindIndexBuffer(m_indexBuffer);
}

// box muller gaussian random variable
Indy::Vector2f GaussianRandomVariable()
{
	float x1, x2, w;
	do {
		x1 = 2.f * (float)rand() / (float)RAND_MAX - 1.f;
		x2 = 2.f * (float)rand() / (float)RAND_MAX - 1.f;
		w = x1 * x1 + x2 * x2;
	} while (w == 0.0 || w > 1.0f);
	w = sqrt(-2.f * log(w) / w);

	return Indy::Vector2f(x1 * w, x2 * w);
}

void Indy::Ocean::createRandomVariables()
{
	m_randomValues = new StorageBuffer();

	const unsigned int numRandomValues = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	Vector2f* randomNumbers = new Vector2f[numRandomValues];

	for (unsigned int y = 0; y < NUM_VERTICES_PER_EDGE; ++y)
		for (unsigned int x = 0; x < NUM_VERTICES_PER_EDGE; ++x)
			randomNumbers[y * NUM_VERTICES_PER_EDGE + x] = GaussianRandomVariable();

	m_randomValues->Create(randomNumbers, sizeof(Vector2f), numRandomValues);
	m_randomValues->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_RANDOM_VARS);

	delete[] randomNumbers;
}

void Indy::Ocean::createhValues()
{
	m_hValues = new StorageBuffer();

	const unsigned int numValues = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	Vector2f* hValues = new Vector2f[numValues * 2]; // we multiply by two to create vector 4s of vector 2s
	m_hValues->Create(hValues, sizeof(Vector2f) * 2, numValues);
	m_hValues->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_HVALUES);
	delete[] hValues;
	
	float* dispersion = new float[numValues];
	m_dispersion = new StorageBuffer();
	m_dispersion->Create(dispersion, sizeof(float), numValues);
	m_dispersion->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_DISPERSIONS);
	delete[] dispersion;

	// dispatch compute to initialize data
	if (Indy::GLSLShader::ComputeShaderSupportAvailable())
	{
		m_computeVertexInitializeProgram->Bind();

		// send the delta time to ensure frame sync
		m_computeVertexInitializeProgram->SetUniformi("N", NUM_VERTICES_PER_EDGE);
		m_computeVertexInitializeProgram->SetUniformf("G", 9.81f);
		m_computeVertexInitializeProgram->SetUniformf("Length", m_length);
		m_computeVertexInitializeProgram->SetUniform2f("W", m_windVec.XY);
		m_computeVertexInitializeProgram->SetUniformf("A", m_phillipsSpectrum);

		m_randomValues->Bind();

		m_hValues->Bind();

		m_dispersion->Bind();

		// distribute work in quads
		int numCompute = (NUM_VERTICES_PER_EDGE + (WORK_GROUP_SIZE / 2)) / WORK_GROUP_SIZE;
		glDispatchCompute(numCompute, numCompute, 1);

		m_hValues->Unbind();

		m_dispersion->Unbind();

		m_randomValues->Unbind();

		m_computeVertexInitializeProgram->Unbind();
	}

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}