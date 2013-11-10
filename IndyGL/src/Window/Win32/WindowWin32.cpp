// Juul Joosten 2013


#include "WindowWin32.h"

#include <iostream>
#include <assert.h>

// OpenGL specific
#include "GL/glew.h"
#include "GL/wglew.h"

namespace Indy
{

	WindowWin32::WindowWin32( void)
		:
	m_isInFocus(false),
	m_isFullscreen(false),
	m_requestQuit(false),
	m_width(0),
	m_height(0),
	m_positionX(0),
	m_positionY(0),
	m_windowTitle(NULL),
	m_windowRect(),
	m_window(),
	m_deviceContext(NULL),
	m_windowHandle(NULL),
	m_moduleHandle(NULL)
	{

	}

	WindowWin32::~WindowWin32( void)
	{

	}


	bool WindowWin32::Create( const unsigned int width, const unsigned int height, 
							  const bool requestFullscreen, 
							  const char* windowTitle, const unsigned int bits)
	{
		m_width = width;
		m_height = height;

		m_isFullscreen = requestFullscreen;

		DWORD dwExStyle;	// extended window style
		DWORD dwStyle;		// window style

		// setup window rectangle (size)
		m_windowRect.left = (long)0;
		m_windowRect.right = (long)width;
		m_windowRect.top = (long)0;
		m_windowRect.bottom = (long)height;

		m_moduleHandle = GetModuleHandle(NULL);

		// setup window parameters
		m_window.cbSize = sizeof(WNDCLASSEX);
		m_window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		m_window.lpfnWndProc = WindowWin32::StaticWndProc; 
		m_window.cbClsExtra = 0;
		m_window.cbWndExtra = 0;
		m_window.hInstance = m_moduleHandle;
		m_window.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		m_window.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_window.hbrBackground = NULL;
		m_window.lpszMenuName = NULL;
		m_window.lpszClassName = "Window";

		// create window class
		if (!RegisterClassEx(&m_window))
		{
			printf("Failed to rigister the window class", (LPWSTR)"ERROR", MB_OK | MB_ICONINFORMATION);
			return false;
		}

		// setup fullscreen parameters
		if (m_isFullscreen)
		{
			DEVMODE dmScreenSettings; // info about device init
			memset( &dmScreenSettings, 0, sizeof( dmScreenSettings));
			dmScreenSettings.dmSize = sizeof( dmScreenSettings);
			dmScreenSettings.dmPelsWidth = width;
			dmScreenSettings.dmPelsHeight = height;
			dmScreenSettings.dmBitsPerPel = bits; // bits per pixel
			dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// check if display settings can be switched
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) 
			{
				printf("The requested FullScreen Mode is not supported by your video card use window mode instead!");
				m_isFullscreen = false;
			}
		}

		if (m_isFullscreen)
		{
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			ShowCursor(false); // Hide mouse pointer
		}
		else
		{
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			dwStyle = WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx(&m_windowRect, dwStyle, false, dwExStyle);

		if ( !(m_windowHandle = CreateWindowEx(	dwExStyle, 
												"Window",
												windowTitle,
												WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
												0,
												0, 
												m_windowRect.right - m_windowRect.left,
												m_windowRect.bottom - m_windowRect.top,
												NULL,
												NULL,
												m_moduleHandle,
												this))) // last var is a pointer to the class that receives the lpfnWndProc
		{
			printf("Window creation Error");
			return false;
		}
	
		if (!(m_deviceContext = GetDC( m_windowHandle))) // get handle to device context for client erea of specified window or for the entire screen
		{
			printf("Cant create a Device Context.");
			return false;
		}

		ShowWindow( m_windowHandle, SW_SHOW);
		UpdateWindow( m_windowHandle);      
		SetForegroundWindow(m_windowHandle);
		SetFocus(m_windowHandle);
		m_isInFocus = true;

		return true;
	}

	void WindowWin32::Destroy( void)
	{
		if ( m_isFullscreen)
		{
			ChangeDisplaySettings( NULL, 0);
			ShowCursor(true);
		}

		// delete window
		if ( m_deviceContext && (ReleaseDC( m_windowHandle, m_deviceContext) == 0))
			printf("Release Device Context failed");
	
		m_deviceContext = NULL;

		if ( m_windowHandle && (DestroyWindow( m_windowHandle) == 0))
			printf("Couldn't release window!");
	
		m_windowHandle = NULL;

		if ( m_moduleHandle && (UnregisterClass("Window", m_moduleHandle) == 0))
			 printf("Could not unregister Window class!");
	
		m_moduleHandle = NULL;
	}
	

	bool WindowWin32::ChangeToWindowedMode( void)
	{
		assert(false && "Not yet implemented");

		if( !m_isFullscreen)
		{

		}

		return false;
	}

	bool WindowWin32::ChangeToFullScreenMode( void)
	{
		assert(false && "Not yet implemented");

		if( !m_isFullscreen)
		{

		}

		return false;
	}

	bool WindowWin32::SwapBuffer( void)
	{
		return SwapBuffers( m_deviceContext) == 1 ? true : false; 
	}

	// window event handeling
	void WindowWin32::ProcessWindowEvents( void)
	{
		MSG msg;

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // while messages in queue, place them in msg
		{
			TranslateMessage( &msg); // process message

			// handle custom actions on window messages
			handleCustomWindowMessages( msg);

			DispatchMessage( &msg); 
		}
	}

	LRESULT CALLBACK WindowWin32::StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowWin32* window = NULL;

		if( uMsg == WM_CREATE)
		{
			window = (WindowWin32*)( (LPCREATESTRUCT)lParam)->lpCreateParams; // get the window pointer, stored in the last param of CreateWindowEx

			SetWindowLongPtr( hWnd, GWL_USERDATA, (LONG_PTR)window); // associate window pointer with hwnd for events to acces
		}
		else
		{
			window = (WindowWin32*)GetWindowLongPtr( hWnd, GWL_USERDATA);

			if( !window) 
				return DefWindowProc( hWnd, uMsg, wParam, lParam);    
		}

		return window->WndProc( hWnd, uMsg, wParam, lParam); // call local wndproc method 
	}

	LRESULT WindowWin32::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch( uMsg)
		{
			case WM_CREATE: // window creation
			{
				m_requestQuit = false;
				return 0;
			}
			break;
			case WM_CLOSE: // windows is closing
			{
				// todo: make this optional?
				if ( MessageBox( hWnd, "Are you sure you want to close this application?", "Application Quit Message", MB_YESNO) == IDNO)
					return 0;

				// cleanup and close game
				m_requestQuit = true;

				Destroy();
				PostQuitMessage(0); // send a WM_QUIT message
			
				return 0;
			}
			break;
			case WM_DESTROY: // window destroy
			{
				// terminate application

				PostQuitMessage(0); // send a WM_QUIT message

				return 0;
			}
			break;
			case WM_SIZE:
			{
				m_width = (unsigned int)LOWORD(lParam);
				m_height = (unsigned int)HIWORD(lParam);
			}
			break;
			case WM_MOVING:
			{
				RECT* rectangle = (RECT*)lParam;
				m_positionX = (unsigned int)rectangle->left;
				m_positionY	= (unsigned int)rectangle->top;
			}
			break;
		}

		return DefWindowProc( hWnd, uMsg, wParam, lParam);
	}
}