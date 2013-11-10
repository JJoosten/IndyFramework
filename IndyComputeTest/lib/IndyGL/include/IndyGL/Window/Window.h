// Juul Joosten 2013

#pragma once

namespace Indy
{
	class Window
	{

	public:
		virtual bool Create( const unsigned int width, const unsigned int height, 
							 const bool requestFullscreen, const char* windowTitle, 
							 const unsigned int bits) = 0;

		virtual void Destroy( void) = 0;
		
		virtual bool ChangeToWindowedMode( void) = 0;

		virtual bool ChangeToFullScreenMode( void) = 0;

		virtual void ProcessWindowEvents( void) = 0;

		virtual bool SwapBuffer( void) = 0;


		/* --- Getters & Setters --- */
		virtual void SetWindowTitle( const wchar_t* windowTitle) const = 0;

		virtual bool IsInFocus( void) const = 0; 

		virtual bool IsFullscreen( void) const = 0; 

		virtual bool IsQuitRequested( void) const = 0;

		virtual unsigned int GetWidth( void) const = 0;

		virtual unsigned int GetHeight( void) const  = 0;

		virtual unsigned int GetPositionX( void) const = 0;
		
		virtual unsigned int GetPositionY( void) const = 0;

	};
};