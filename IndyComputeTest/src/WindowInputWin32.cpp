// Juul Joosten 2013

#include "WindowInputWin32.h"
#include <IndyCore/Input/Keyboard.h>

namespace Indy
{
	WindowInputWin32::WindowInputWin32( Keyboard& keyboard)
		:
	WindowWin32()
	,m_keyboard(keyboard)
	{

	}

	WindowInputWin32::~WindowInputWin32( void)
	{

	}

	void WindowInputWin32::handleCustomWindowMessages( MSG windowsMessage)
	{
		switch( windowsMessage.message)
		{
			// keyboard
			case WM_KEYDOWN:
			{
				m_keyboard.SetKeyDown((KeyboardKeys::KeyboardKey)windowsMessage.wParam);
			}
			break;
			case WM_KEYUP:
			{
				m_keyboard.SetKeyUp((KeyboardKeys::KeyboardKey)windowsMessage.wParam);
			}
			break;
			
			// mouse
			case WM_LBUTTONDOWN:
			{
				// left button
			}
			break;
			case WM_LBUTTONUP:
			{
				// left button
			}
			break;
			case WM_RBUTTONDOWN:
			{
				// right button
			}
			break;	
			case WM_RBUTTONUP:
			{
				// right button
			}
			break;
			case WM_MBUTTONDOWN:
			{
				// middle button
			}
			break;
			case WM_MBUTTONUP:
			{
				// middle button
			}
			break;
			case WM_MOUSEMOVE:
			{
				// mouse move
			}
			break;
		}
	}
};