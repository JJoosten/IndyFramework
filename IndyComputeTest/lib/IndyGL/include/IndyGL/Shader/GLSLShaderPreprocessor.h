// Juul Joosten 2013

#pragma once

#include <IndyCore/CoreDefines.h>

namespace Indy
{
	struct ShaderSourceData
	{
		char* Source;
		unsigned int SourceSizeInBytes;
		bool WasSourceFound;
	};


	/* The following tokens are currently preprocessed:
		- #include (local to shader file)
			- Make sure that if uncommented // is directly attached to #include ( //#include )
		- Multiple includes of same file will not be included anymore after the first time!

		returns preprocessed source 
	*/
	ShaderSourceData GLSLShaderPreprocessor( const char* const shaderFile,
											 const char** const previousIncludes = NULL, 
											 const unsigned int numPreviousIncludes = 0);

}