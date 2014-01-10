#include "Assert.h"

#include <cstdio>
#include <cstdarg>

namespace Indy
{
	Assert::Assert( const char* const file, const unsigned int line, const char* const format, ...)
	{
		char tmpBuffer[2048];
		printf("--- ASSERT ---\n");
		printf("Assert in %s at line %u\n", file, line);

		va_list argptr;
		va_start(argptr, format);
		vsprintf_s(tmpBuffer, format, argptr);
		va_end(argptr);
		printf("%s \n", tmpBuffer);
	}

	Assert::~Assert( void)
	{
		// insert an extra white space to create some space
		printf("\n");
	}

	
	Assert& Assert::Variable( const char* const varName, bool var)
	{
		printf("%s %s\n", varName, var == true ? "true" : "false");
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, char var)
	{
		printf("%s %d\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, unsigned char var)
	{
		printf("%s %u\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, short var)
	{
		printf("%s %d\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, unsigned short var)
	{
		printf("%s %u\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, int var)
	{
		printf("%s %d\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, unsigned int var)
	{
		printf("%s %u\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, float var)
	{
		printf("%s %f\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, double var)
	{
		printf("%s %f\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, const char* var)
	{
		printf("%s %s\n", varName, var);
		return *this;
	}

	Assert& Assert::Variable( const char* const varName, char* var)
	{
		printf("%s %s\n", varName, var);
		return *this;
	}
	
	Assert& Assert::Variable( const char* const varName, void* var)
	{
		printf("%s %p\n", varName, var);
		return *this;
	}
	
	Assert& Assert::Variable( const char* const varName, const void* var)
	{
		printf("%s %p\n", varName, var);
		return *this;
	}
}