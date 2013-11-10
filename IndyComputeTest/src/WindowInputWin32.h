// Juul Joosten 2013

#pragma once

#include <IndyGL/Window/Win32/WindowWin32.h>

namespace Indy
{
	class Keyboard;
	class WindowInputWin32 : public WindowWin32
	{

	public:
		WindowInputWin32( Keyboard& keyboard);
		virtual ~WindowInputWin32( void);

	private:
		// override from WindowWin32
		virtual void handleCustomWindowMessages( MSG windowsMessage);

	private:
		Keyboard& m_keyboard;
	};
};