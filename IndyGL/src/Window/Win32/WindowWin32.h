// Juul Joosten 2013

#pragma once
#include "../Window.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
	#include <Windows.h>
#elif
	#error This Window class does not support the current platform
#endif

namespace Indy
{
	class WindowWin32 : public Window
	{

	public:
		WindowWin32( void);
		virtual ~WindowWin32( void);

		bool	Create( const unsigned int width, const unsigned int height, 
						const bool requestFullscreen, const char* windowTitle, 
						const unsigned int bits);

		void Destroy( void);


		bool ChangeToWindowedMode( void);

		bool ChangeToFullScreenMode( void);

		bool SwapBuffer( void);


		// process windows events
		void ProcessWindowEvents( void);
		static LRESULT CALLBACK StaticWndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK WndProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);


		/* --- Getters & Setters --- */
		void SetWindowTitle( const wchar_t* windowTitle) const { SetWindowText( m_windowHandle, (LPCTSTR)windowTitle); }

		bool IsInFocus( void) const { return m_isInFocus; }

		bool IsFullscreen( void) const { return m_isFullscreen; }

		bool IsQuitRequested( void) const { return m_requestQuit; }

		unsigned int GetWidth( void) const { return m_width; }

		unsigned int GetHeight( void) const { return m_height; };

		unsigned int GetPositionX( void) const { return m_positionX; }

		unsigned int GetPositionY( void) const  { return m_positionY; }


		const HDC GetDeviceContext( void) const { return m_deviceContext; }



	private:
		// override to handle input and other window messages
		virtual void handleCustomWindowMessages( MSG windowsMessage) {}


	protected:
		bool				m_requestQuit; // set this to true if you want to close the window

	private: 		
		bool				m_isInFocus;
		bool				m_isFullscreen;
		char*				m_windowTitle;
		unsigned int		m_width;
		unsigned int		m_height;
		unsigned int		m_positionX;
		unsigned int		m_positionY;

		
		// windows specific variables
		RECT		m_windowRect;
		WNDCLASSEX	m_window;
		HDC			m_deviceContext;
		HWND		m_windowHandle;
		HINSTANCE	m_moduleHandle;
	};
};