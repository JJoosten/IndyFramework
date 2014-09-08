#pragma once

#include "../Utilities/UtilityDefines.h"

// --- BREAKPOINT MACRO ---
#ifdef _DEBUG
#ifdef _MSC_VER // check if visual studio compiler is used
	#include <intrin.h>
	#define BREAKPOINT_ASSERT __debugbreak()
#else
	#error BREAKPOINT_ASSERT define not implemented on this platform!
#endif
#endif

namespace Indy
{
	// based on Stefan Reinalter assert http://www.altdevblogaday.com/2011/10/12/upgrading-assert-using-the-preprocessor/ & http://altdevblogaday.com/wp-content/uploads/2011/10/Assert.txt and 
	// macros http://molecularmusings.wordpress.com/2011/07/12/a-plethora-of-macros/ 
	class Assert
	{

	public:
		Assert( const char* const file, const unsigned int line, const char* const format, ...);
		~Assert( void);

		Assert& Variable( const char* const varName, bool var);
		Assert& Variable( const char* const varName, char var);
		Assert& Variable( const char* const varName, unsigned char var);
		Assert& Variable( const char* const varName, short var);
		Assert& Variable( const char* const varName, unsigned short var);
		Assert& Variable( const char* const varName, int var);
		Assert& Variable( const char* const varName, unsigned int var);
		Assert& Variable( const char* const varName, float var);
		Assert& Variable( const char* const varName, double var);
		Assert& Variable( const char* const varName, char* var);
		Assert& Variable( const char* const varName, void* var);
	};

// --- VARS addition macro ---
#ifdef _DEBUG
	#define ASSERT_EXPAND_ARGS_0(op, empty)
	#define ASSERT_EXPAND_ARGS_1(op, arg1)												op(arg1, 0)
	#define ASSERT_EXPAND_ARGS_2(op, arg1, arg2)										op(arg1, 0)	op(arg2, 1)
	#define ASSERT_EXPAND_ARGS_3(op, arg1, arg2, arg3)									op(arg1, 0) op(arg2, 1) op(arg3, 2)
	#define ASSERT_EXPAND_ARGS_4(op, arg1, arg2, arg3, arg4)							op(arg1, 0) op(arg2, 1) op(arg3, 2) op(arg4, 3)
	#define ASSERT_EXPAND_ARGS_5(op, arg1, arg2, arg3, arg4, arg5)						op(arg1, 0) op(arg2, 1) op(arg3, 2) op(arg4, 3) op(arg5, 4)
	#define ASSERT_EXPAND_ARGS_6(op, arg1, arg2, arg3, arg4, arg5, arg6)				op(arg1, 0) op(arg2, 1) op(arg3, 2) op(arg4, 3) op(arg5, 4) op(arg6, 5)
	#define ASSERT_EXPAND_ARGS_7(op, arg1, arg2, arg3, arg4, arg5, arg6, arg7)			op(arg1, 0) op(arg2, 1) op(arg3, 2) op(arg4, 3) op(arg5, 4) op(arg6, 5) op(arg7, 6)
	#define ASSERT_EXPAND_ARGS_8(op, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)	op(arg1, 0) op(arg2, 1) op(arg3, 2) op(arg4, 3) op(arg5, 4) op(arg6, 5) op(arg7, 6) op(arg8, 7)

	//ASSERT_VARIABLE_CALL(variable, n) .Variable(#variable, variable)
	//ASSERT_VARIABLES_CONCAT(...) CONCATINATE(ASSERT_EXPAND_ARGS_, VAR_NUM_ARGS(__VA_ARGS__)) PASS_ARGS(op, __VA_ARGS__)
#else

#endif

// --- ASSERT MACRO ---
#ifdef _DEBUG
	#define ASSERT_WITHOUT_PARAMS(condition, format, ...) (condition) ? (void)true : Assert(__FILE__, __LINE__, "Assertion \"" #condition "\" failed.\n" format, __VA_ARGS__)
	#define ASSERT(condition, format, ...) ASSERT_WITHOUT_PARAMS(Vcondition, format, __VA_ARGS__) ASSERT_VARIABLES_CONCAT, BREAKPOINT_ASSERT
#else
	#define ASSERT(condition, format, ...) (void)0
#endif
}