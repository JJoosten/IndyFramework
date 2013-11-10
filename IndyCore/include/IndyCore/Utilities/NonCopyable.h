/* Juul Joosten 2013

	--- INFO ---
	Inherit to make sure class can not be copied 
	by use of copy constructor or assignment operator
*/

#pragma once

namespace Indy
{
	class NonCopyable
	{

	protected:
		NonCopyable( void) {}
		~NonCopyable( void) {}

	private:
		// copy ctor
		NonCopyable( const NonCopyable& cpyCtor);

		// assignment operator overload
		const NonCopyable& operator=( const NonCopyable& assignmentOperator);
	};
};