#pragma once

// based on http://molecularmusings.wordpress.com/2011/07/12/a-plethora-of-macros/

#define STRINGIFY_HELPER_ARG1(value)	#value
#define STRINGIFY(value)				STRINGIFY_HELPER_ARG1(value)

#define CONCATINATE_HELPER_ARG1(a,b)	a##b
#define CONCATINATE_HELPER_ARG2(a,b)	CONCATINATE_HELPER_ARG1(a,b)
#define CONCATINATE(a,b)				CONCATINATE_HELPER_ARG2(a,b)


// ME_VA_NUM_ARGS() is a very nifty macro to retrieve the number of arguments handed to a variable-argument macro
// unfortunately, VS 2010 still has this compiler bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
	#define VAR_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
	#define VAR_NUM_ARGS_REVERSE_SEQUENCE            10, 9, 8, 7, 6, 5, 4, 3, 2, 1
	#define LEFT_PARENTHESIS (
	#define RIGHT_PARENTHESIS )
	#define VAR_NUM_ARGS(...)                        VAR_NUM_ARGS_HELPER LEFT_PARENTHESIS __VA_ARGS__, VAR_NUM_ARGS_REVERSE_SEQUENCE RIGHT_PARENTHESIS
#else
	#define VAR_NUM_ARGS(...)                        VAR_NUM_ARGS_HELPER(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
	#define VAR_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#endif
 
// ME_PASS_VA passes __VA_ARGS__ as multiple parameters to another macro, working around the above-mentioned bug
#if _MSC_VER >= 1400
	#define PASS_VAR(...)                           LEFT_PARENTHESIS __VA_ARGS__ RIGHT_PARENTHESIS
#else
	#define PASS_VAR(...)                            (__VA_ARGS__)
#endif