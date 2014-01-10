#include "IO.h"

#include <stdio.h>

namespace Indy
{

	bool LoadFile( const char* const filePath, FileData& fileDataOUT)
	{
		FILE* file = fopen(filePath, "rb");

		if(file == NULL)
			return false;
		
		fseek(file, 0, SEEK_END);
		fileDataOUT.SizeInBytes = ftell(file);
		fseek(file, 0, SEEK_SET);

		fileDataOUT.Data = new char[fileDataOUT.SizeInBytes];
		fread(fileDataOUT.Data, 1, fileDataOUT.SizeInBytes, file); 

		fclose(file);

		return true;
	}

	bool WriteToFile( const char* const filePath, void* data, unsigned int dataSizeInBytes)
	{
		FILE* file = fopen(filePath, "wb");
		
		if(file == NULL)
			return false;
		
		fwrite( data, 1, dataSizeInBytes, file);

		fclose(file);

		return true;
	}

	bool AppendToFile( const char* const filePath, void* data, unsigned int dataSizeInBytes)
	{
		FILE* file = fopen(filePath, "ab");
		
		if(file == NULL)
			return false;
		
		fwrite( data, 1, dataSizeInBytes, file);

		fclose(file);

		return true;
	}

}