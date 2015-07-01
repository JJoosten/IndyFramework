#pragma once

#include <IndyCore\Math\Vector3.h>
#include <IndyCore\Math\Vector2.h>

namespace Indy
{
	class VertexArrayObject;
	class GLSLShader;
	class StorageBuffer;
	class GLSLShaderProgram;
	class Camera;
	class IndexBuffer;
	class Ocean
	{
		struct OceanVertex
		{
			Vector3f Pos;
			float OrigX;
			Vector3f Norm;
			float OrigZ;
		};

	public:
		Ocean(float length, Indy::Vector2f windVec, float phillipsSpectrum);
		~Ocean();

		// update using compute
		void UpdateFrame(const double deltaTimeSec);

		// draw updated vertex buffer
		void DrawFrame(const Camera& camera);

	private:
		void initializeShaders();
		void createOceanMesh();
		void createRandomVariables();
		void createhValues();

	private:
		StorageBuffer*			m_vertices;
		StorageBuffer*			m_randomValues;
		StorageBuffer*			m_hValues;
		StorageBuffer*			m_dispersion;
		IndexBuffer*			m_indexBuffer;
		VertexArrayObject*		m_vao;
		GLSLShader*				m_computeVertexTransform;
		GLSLShaderProgram*		m_computeVertexTransformProgram;
		GLSLShader*				m_computeVertexInitialize;
		GLSLShaderProgram*		m_computeVertexInitializeProgram; 
		GLSLShader*				m_vertexShader;
		GLSLShader*				m_fragmentShader;
		GLSLShaderProgram*		m_shadingProgram;

		Vector2f				m_windVec;
		float					m_length;
		float					m_phillipsSpectrum;
		float					m_elapsedTimeInSec;
	};
}