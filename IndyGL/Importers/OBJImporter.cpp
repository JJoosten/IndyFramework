#include "OBJImporter.h"
#include <stdio.h>
#include <memory>

namespace Indy
{
	bool LoadOBJFile( const char* const objFile, OBJModelData& loadedModelDataINOUT)
	{
		// open file
		FILE* file = NULL;
		file = fopen(objFile, "rb");
		if(file == NULL)
			return false;

		// read file size
		fseek(file, 0, SEEK_END);
		unsigned int fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		// load file in memory
		unsigned char* fileData = new unsigned char[fileSize];
		fread( fileData, 1, fileSize, file);
		fclose(file);

		// go over file to search for vertices, num vertices, uvs, normals, indices, num indices
		unsigned int numMeshes = 0;
		for( unsigned int i = 0; i < fileSize; ++i)
		{

		}

		// create data structures

		// go over file to retrieve vertices, num vertices, uvs, normals, indices, num indices
		for( unsigned int i = 0; i < fileSize; ++i)
		{

		}

		// delete local file data
		delete[] fileData;

		return true;
	}
}