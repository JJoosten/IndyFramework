#pragma once

namespace Indy
{
	class VertexArrayObject;
	class GLSLShader;
	class StorageBuffer;
	class GLSLShaderProgram;
	class Camera;

	class Ocean
	{
	public:
		Ocean();
		~Ocean();

		// update using compute
		void UpdateFrame(const double deltaTimeSec);

		// draw updated vertex buffer
		void DrawFrame(const Camera& camera);

	private:
		void initializeShaders();
		void createOceanMesh();

	private:
		StorageBuffer*			m_vertices;
		VertexArrayObject*		m_vao;
		GLSLShader*				m_computeVertexTransform;
		GLSLShaderProgram*		m_computeVertexTransformProgram;
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_shadingProgram;

	};
}