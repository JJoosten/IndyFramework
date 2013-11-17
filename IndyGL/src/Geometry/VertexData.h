// Juul Joosten 2013

#pragma once

namespace Indy
{
	template<typename T>
	struct VertexPos1
	{
		T X;
	};
	
	template<typename T>
	struct VertexPos2 : public VertexPos1<T>
	{
		T Y;
	};
	
	template<typename T>
	struct VertexPos3 : public VertexPos2<T>
	{
		T Z;
	};

	template<typename T>
	struct VertexPos4 : public VertexPos3<T>
	{
		T W;
	};
}
