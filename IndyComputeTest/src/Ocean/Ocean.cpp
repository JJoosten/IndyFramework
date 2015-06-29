#include "Ocean.h"
#include "../Paths.h"

#include <IndyCore\CoreDefines.h>
#include <IndyGL\Camera\Camera.h>
#include <IndyGL\Shader\GLSLShader.h>
#include <IndyGL\Shader\GLSLShaderProgram.h>
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
,m_length(length)
,m_windVec(windVec)
,m_phillipsSpectrum(phillipsSpectrum)
{
	createOceanMesh();

	createRandomVariables();

	createhValues();

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

	m_randomValues->Destroy();
	delete m_randomValues;

	m_hValues->Destroy();
	delete m_hValues;

}

int g_frame = 0;
void Indy::Ocean::UpdateFrame(const double deltaTimeSec)
{
	static float time = 0;
	time += deltaTimeSec;

	g_frame++;

	if (g_frame != 120)
		return;

	g_frame = 0;

	if (Indy::GLSLShader::ComputeShaderSupportAvailable())
	{
		m_computeVertexTransformProgram->Bind();

		// send the delta time to ensure frame sync
		m_computeVertexTransformProgram->SetUniformf("DeltaTimeSec", deltaTimeSec);
		m_computeVertexTransformProgram->SetUniformi("N", NUM_VERTICES_PER_EDGE);
		m_computeVertexTransformProgram->SetUniform2f("W", m_windVec.XY);
		m_computeVertexTransformProgram->SetUniformf("G", 9.81f);
		m_computeVertexTransformProgram->SetUniformf("A", m_phillipsSpectrum);
		m_computeVertexTransformProgram->SetUniformf("Length", m_length);

		m_vertices->Bind();

		m_randomValues->Bind();

		m_hValues->Bind();

		// calculate num vertices to transform
		unsigned int arraySize = m_vertices->GetArraySize();

		// distribute work in quads
		int sqr = (int)ceil(std::sqrtf((float)arraySize));

		glDispatchCompute(sqr / WORK_GROUP_SIZE, sqr / WORK_GROUP_SIZE, 1);

		m_hValues->Unbind();

		m_randomValues->Unbind();

		m_vertices->Unbind();

		m_computeVertexTransformProgram->Unbind();
	}
}

void Indy::Ocean::DrawFrame(const Indy::Camera& camera)
{
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	m_shadingProgram->Bind();

	m_shadingProgram->SetUniform4x4f("ViewMat", camera.GetViewMatrix(), false);
	m_shadingProgram->SetUniform4x4f("ProjMat", camera.GetProjectionMatrix(), false);

	m_vao->Bind();

	glPointSize(10.0f);

	glDrawArrays(GL_POINTS, 0, m_vertices->GetArraySize());

	m_vao->Unbind();

	m_shadingProgram->Unbind();

	//m_vertices->PullFromGPU();
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

	const unsigned int numVertices = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	OceanVertex vertices[numVertices]; // test 64*64 vertices

	// spread the vertices
	for (unsigned int y = 0; y < NUM_VERTICES_PER_EDGE; ++y)
	{
		for (unsigned int x = 0; x < NUM_VERTICES_PER_EDGE; ++x)
		{
			const unsigned int index = y * NUM_VERTICES_PER_EDGE + x;

			float origX = ((float)x - (float)NUM_VERTICES_PER_EDGE / 2.0f) * m_length / (float)NUM_VERTICES_PER_EDGE;
			float origZ = ((float)y - (float)NUM_VERTICES_PER_EDGE / 2.0f) * m_length / (float)NUM_VERTICES_PER_EDGE;

			vertices[index].Pos = Vector3f(origX, 0, origZ);
			vertices[index].OrigX = origX;
			vertices[index].Norm = Vector3f(0, 1, 0);
			vertices[index].OrigZ = origZ;
		}
	}

	m_vertices = new StorageBuffer();
	m_vertices->Create(vertices, sizeof(OceanVertex), numVertices);

	char* offsetInBytes = (char*)sizeof(OceanVertex);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertices->GetID());
	m_vao->VertexAttributePointer(0, 3, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), 0);
	m_vao->VertexAttributePointer(1, 3, GL_FLOAT, Indy::NormalizeFixedPointValues::FALSE, sizeof(OceanVertex), offsetInBytes);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vertices->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_VERTICES);
}

Indy::Vector2f GaussianRandomVariable()
{
	float x1, x2, w;
	do {
		x1 = 2.f * (float)rand() / RAND_MAX - 1.f;
		x2 = 2.f * (float)rand() / RAND_MAX - 1.f;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.f);
	w = sqrt((-2.f * log(w)) / w);

	return Indy::Vector2f(x1 * w, x2 * w);
}

static float g_length;
static Indy::Vector2f g_W;
static float g_A;
void Indy::Ocean::createRandomVariables()
{
	// QQQ
	g_length = m_length;
	g_W = m_windVec;
	g_A = m_phillipsSpectrum;

	m_randomValues = new StorageBuffer();

	const unsigned int numRandomValues = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	Vector2f randomNumbers[numRandomValues];

	for (unsigned int y = 0; y < NUM_VERTICES_PER_EDGE; ++y)
	{
		for (unsigned int x = 0; x < NUM_VERTICES_PER_EDGE; ++x)
		{
			randomNumbers[y * NUM_VERTICES_PER_EDGE + x] = GaussianRandomVariable();
		}
	}

	m_randomValues->Create(randomNumbers, sizeof(Vector2f), numRandomValues);
	m_randomValues->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_RANDOM_VARS);
}

float dispersion(int n_prime, int m_prime) 
{
	float w_0 = 2.0f * M_PI / 200.0f;
	float kx = M_PI * (2 * n_prime - NUM_VERTICES_PER_EDGE) / g_length;
	float kz = M_PI * (2 * m_prime - NUM_VERTICES_PER_EDGE) / g_length;
	return floor(sqrt(9.81f * sqrt(kx * kx + kz * kz)) / w_0) * w_0;
}

float phillips(int n_prime, int m_prime) {
	Indy::Vector2f k(M_PI * (2 * n_prime - NUM_VERTICES_PER_EDGE) / g_length, M_PI * (2 * m_prime - NUM_VERTICES_PER_EDGE) / g_length);
	float k_length = Indy::GetLength(k);
	if (k_length < 0.000001) return 0.0;

	float k_length2 = k_length  * k_length;
	float k_length4 = k_length2 * k_length2;

	float k_dot_w = Indy::Dot(Indy::Vector2f(k).Normalize(), Indy::Vector2f(g_W).Normalize());
	float k_dot_w2 = k_dot_w * k_dot_w;

	float w_length = Indy::GetLength(g_W);
	float L = w_length * w_length / 9.81f;
	float L2 = L * L;

	float damping = 0.001;
	float l2 = L2 * damping * damping;

	return g_A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
}

Indy::Vector2f hTilde_0(int n_prime, int m_prime) 
{
	Indy::Vector2f r = GaussianRandomVariable();
	return r * sqrt(phillips(n_prime, m_prime) / 2.0f);
}

void Indy::Ocean::createhValues()
{
	m_hValues = new StorageBuffer();

	const unsigned int numHValues = NUM_VERTICES_PER_EDGE * NUM_VERTICES_PER_EDGE;
	Vector2f hValues[numHValues * 2]; // we multiply by two to create vector 4s of vector 2s
	for ( int y = 0; y < NUM_VERTICES_PER_EDGE; ++y)
	{
		for ( int x = 0; x < NUM_VERTICES_PER_EDGE; ++x)
		{
			int index = y * NUM_VERTICES_PER_EDGE * 2 + x * 2;

			Vector2f conj = hTilde_0(-x, -y);

			hValues[index + 0] = hTilde_0(x,y);
			hValues[index + 1] = Vector2f(conj.X, -conj.Y);
		}
	}

	m_hValues->Create(hValues, sizeof(Vector2f) * 2, numHValues);
	m_hValues->SetShaderLayoutIndex(SHADER_LAYOUT_INDEX_HVALUES);
}