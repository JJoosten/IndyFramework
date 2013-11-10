// Juul Joosten 2013

#include "GLSLShaderPreprocessor.h"

#include <ios>
#include <queue>
#include <vector>

namespace Indy
{
	ShaderSourceData GLSLShaderPreprocessor( const char* const sourceFile, 
											 const char** const previousIncludes /* = NULL*/, 
											 const unsigned int numPreviousIncludes /* = 0 */)
	{
		// get local path from sourceFile
		const int stringLength = strlen(sourceFile);

		// find first \ or /
		int lengthOfPath = 0;
		for( int i = 0; i < stringLength; ++i)
			if( sourceFile[i] == '\\' || sourceFile[i] == '/')
				lengthOfPath = i + 1; // add 1 to compensate for that i starts from 0

		std::vector<const char*> previouslyIncludedFiles;

		
		ShaderSourceData sourceData;
		memset( &sourceData, 0, sizeof(ShaderSourceData));

		// check for previous includes
		for( unsigned int i = 0; i < numPreviousIncludes; ++i)
		{
			previouslyIncludedFiles.push_back( previousIncludes[i]);

			// only include a file once!
			if( strcmp(previousIncludes[i], sourceFile) == 0)
				return sourceData;
				//BREAKPOINT( Circular dependency in shader #include preprocessor!);
		}


		// get shader source from file
		FILE* file = fopen( sourceFile, "rb");

		if( file == NULL)
			return sourceData;

		fseek(file, 0, SEEK_END);
		sourceData.SourceSizeInBytes = ftell(file);
		fseek(file, 0, SEEK_SET);

		sourceData.Source = new char[sourceData.SourceSizeInBytes];
		sourceData.WasSourceFound = true;
		fread( sourceData.Source, 1, sourceData.SourceSizeInBytes, file);
		fclose(file);


		
		std::queue<int>					locationToInsert;
		std::queue<ShaderSourceData>	sourceDatas;
		unsigned int totalFileSize		= sourceData.SourceSizeInBytes;
	
		for( unsigned int i = 0; i < sourceData.SourceSizeInBytes; ++i)
		{
			if( sourceData.Source[i] != '#')
				continue;

			// strlen("#include") == 8 characters + space and  2 * " = 11 characters
			// we need to add 1 to i to make sure its in the same range as the length index goes from 0 - length - 1
			if( (sourceData.SourceSizeInBytes - (i + 1)) < 11)
				break;

			// when we find #include keyword
			if( strncmp( &sourceData.Source[i], "#include", 8) != 0)
				continue;

			// check if #include is commented out, we check i against 1 to make sure that we have room for 2 slashes (on index 0 and 1)
			if( i > 1 && 
				sourceData.Source[i - 1] == '/' && 
				sourceData.Source[i - 2] == '/')
				continue;

			// capture start of #include keyword so we can blank it out later
			int locationOfIncludeKeyword = i;

			// cache 
			int startOfStringLocation = 0;
			int numQuotationMarks = 0;

			// add 8 to i to jump over the #include characters
			i += 8;
						
			bool terminatorFound = false;
			while( !terminatorFound)
			{
				// when we find end of line but string is not terminated, let glsl compiler give error
				if( sourceData.Source[i] == '\n' || 
					sourceData.Source[i] == '\0')
					BREAKPOINT( #include statement is not correctly closed);

				if( sourceData.Source[i] == '"')
				{
					++numQuotationMarks;

					switch( numQuotationMarks)
					{
						// when string starts, save location of string
						case 1:
							// we add 1 to i to jump over the " character
							startOfStringLocation = i + 1;
						break;

						// when string ends we use it as a path to another shader file
						case 2:
						{
							// we substract 1 from i to skip the end of the string " character
							int endOfString = i - 1;

							// we check the length, we add 1 to account for the indexing beginning from 0
							int length = endOfString - startOfStringLocation + 1;

							if( length <= 0)
								BREAKPOINT( #include string is not valid in shader);

							// we add 1 to the length to make room for a NULL terminator \0
							char* path = new char[lengthOfPath + length + 1];
							if( lengthOfPath > 0)
								memcpy( path, sourceFile, lengthOfPath);

							memcpy( path + lengthOfPath, &sourceData.Source[startOfStringLocation], length);
							path[lengthOfPath + length] = '\0';

							// remove the include line from the shader source (because #include is not supported by GLSL)
							// we add 1 to the end of string to make sure the length covers the last " 
							endOfString += 1;

							// we add 1 to account for index numbers starting from 0 
							int lengthOfTotalIncludeStatement = endOfString - locationOfIncludeKeyword + 1;
							memset( &sourceData.Source[locationOfIncludeKeyword],' ', lengthOfTotalIncludeStatement); 
									
							// recursively go deeper to get the next file data (this gets pasted @ location locationToInsert.front() )
							const char** prevIncludeFiles = NULL;
							if( previouslyIncludedFiles.size() > 0)
								prevIncludeFiles = &previouslyIncludedFiles[0];
							ShaderSourceData newFileSourceData = GLSLShaderPreprocessor( path, prevIncludeFiles, previouslyIncludedFiles.size());

							// we check if the source actually excists
							if( newFileSourceData.WasSourceFound)
							{
								// keep track of line to make sure we can insert included code here
								locationToInsert.push( locationOfIncludeKeyword);

								// push the new file on the stack to be added to the shader
								sourceDatas.push(newFileSourceData);

								// add path to previously included paths so recursive function can check for circular dependencies
								previouslyIncludedFiles.push_back(path);

								// count the total file size so we know how big the final shader is going to be
								totalFileSize += newFileSourceData.SourceSizeInBytes;
							}

							terminatorFound = true;
						}
						break;
					}
				}

				if(!terminatorFound)
					++i;
							
				// end of while loop
			}
		}
		
		if(sourceDatas.size() > 0)
		{
			char* newSource = new char[totalFileSize];
			memcpy( newSource, sourceData.Source, locationToInsert.front());

			unsigned int pastedFileOffset = 0;
			unsigned int lastPasteLocation = 0;
			while( sourceDatas.size() > 0)
			{

				// get data to paste
				ShaderSourceData pasteData = sourceDatas.front();
				sourceDatas.pop();

				// get offset to paste
				unsigned int newPasteLocation = locationToInsert.front();
				locationToInsert.pop();

				// add local offset
				pastedFileOffset += (newPasteLocation - lastPasteLocation);

				memcpy( newSource + pastedFileOffset, pasteData.Source, pasteData.SourceSizeInBytes); 

				// add file offset
				pastedFileOffset += pasteData.SourceSizeInBytes;
				
				lastPasteLocation = newPasteLocation;

				if( sourceDatas.size() > 0)
				{
					newPasteLocation = locationToInsert.front();
					memcpy( newSource + pastedFileOffset,  &sourceData.Source[lastPasteLocation], (newPasteLocation - lastPasteLocation));
				}
				else
				{
					memcpy( newSource + pastedFileOffset, &sourceData.Source[lastPasteLocation], sourceData.SourceSizeInBytes - lastPasteLocation); 
				}

				// clean pastedata 
				delete[] pasteData.Source;
			}

			// delete old source and set new source
			delete[] sourceData.Source;

			sourceData.Source = newSource;
			sourceData.SourceSizeInBytes = totalFileSize;

			/*
			FILE* output = fopen("c://shader.txt", "wb+");
			fwrite(sourceData.Source, 1, sourceData.SourceSizeInBytes, output);
			fclose(output);
			*/

			while( previouslyIncludedFiles.size() > numPreviousIncludes)
			{
				unsigned int index = numPreviousIncludes;
				delete[] previouslyIncludedFiles[numPreviousIncludes];
				previouslyIncludedFiles.erase(previouslyIncludedFiles.begin() + numPreviousIncludes);
			}
		}

		return sourceData;
	}
}