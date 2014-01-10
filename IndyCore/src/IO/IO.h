// Juul Joosten 2013

#pragma once

namespace Indy
{
	struct FileData
	{
		unsigned int SizeInBytes;
		void* Data;
	};

	bool LoadFile( const char* const filePath, FileData& fileDataOUT);

	bool WriteToFile( const char* const filePath, void* data, unsigned int dataSizeInBytes);

	bool AppendToFile( const char* const filePath, void* data, unsigned int dataSizeInBytes);


}