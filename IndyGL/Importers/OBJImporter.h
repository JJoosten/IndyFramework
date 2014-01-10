#pragma once

namespace Indy
{
	struct OBJMeshData
	{
		unsigned int NumVertices;
		unsigned int NumIndices;
		float* Vertices;
		float* Normals;
		float* UVs;
		unsigned int* Indices;
	};

	struct OBJModelData
	{
		unsigned int NumMeshes;
		unsigned int NumMaterialFiles;
		OBJMeshData* Meshes;
		char*		 MaterialFiles;
	};

	struct MTLData
	{
		float AmbientColor[3];
		float DiffuseColor[3];
		float SpecularColor[3];
		float SpecularCoeff;
		float Alpha;
		unsigned int IlluminationModel;
		char* AmbientTexture;
		char* DiffuseTexture;
		char* SpecularTexture;
		char* SpecularHighlightTexture;
		char* AlphaTexture;
		char* BumpTexture;
		char* DisplacementTexture;
		char* DecalTexture;
	};

	struct MTLSet
	{
		unsigned int NumMaterials;
		MTLData* Materials;
	};

	bool LoadOBJFile( const char* const objFile, OBJModelData& loadedModelDataINOUT);
}