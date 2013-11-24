// Juul Joosten 2013

#pragma once

namespace Indy
{
	struct VertexPos1
	{
		float PosX;
	};

	struct VertexPos2 : public VertexPos1
	{
		float PosY;
	};

	struct VertexPos3 : public VertexPos2
	{
		float PosZ;
	};
	
	struct VertexPos2UV : public VertexPos2
	{
		float U;
		float V;
	};

	struct VertexPos3UV : public VertexPos3
	{
		float U;
		float V;
	};

	struct VertexPos1Color : public VertexPos1
	{
		unsigned int RGBA;
	};

	struct VertexPos2Color : public VertexPos2
	{
		unsigned int RGBA;
	};

	struct VertexPos3Color : public VertexPos3
	{
		unsigned int RGBA;
	};
 
	struct VertexPos2UVColor : public VertexPos2UV
	{
		unsigned int RGBA;
	};

	struct VertexPos3UVColor : public VertexPos3UV
	{
		unsigned int RGBA;
	};
}
