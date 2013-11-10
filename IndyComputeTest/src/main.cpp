// Juul Joosten 2013

#include "Game.h"
#include <IndyCore/Time/Timer.h>
#include <IndyCore/Input/Keyboard.h>
#include <IndyGL/Context/Win32/GLContextWin32.h>
#include "WindowInputWin32.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// enable visual leak detector (create VLD_DIR environment variable to point to install folder)
#ifdef VLD_ENABLED
#include <vld.h>
#endif

// force NVIDIA GPU from driver Release 302 and up
extern "C" 
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}


void initializeGlew( void)
{
	GLenum error = glewInit();
	if (error != GLEW_OK)
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

int main( int argc, char** argv)
{
	char* applicationPath = argv[0];
	
	unsigned int resolutionX = SCREEN_WIDTH;
	unsigned int resolutionY = SCREEN_HEIGHT;

	// change resolution from command args
	if(argc > 2)
	{
		char* resX = argv[1];
		resolutionX = atoi(resX);
		char* resY = argv[2];
		resolutionY = atoi(resY);
	}

	Indy::Keyboard keyboard;

	Indy::Window* window = NULL;
#ifdef WIN32
		window = new Indy::WindowInputWin32(keyboard);
#else
	exit(0);
#endif
	window->Create( resolutionX, resolutionY, false, "Indy Compute Shader Test", 32);

	Indy::OpenGLInfo openGLInfo;
	openGLInfo.MajorVersion = 3;
	openGLInfo.MinorVersion = 1;
	Indy::GLContext* glContext = NULL;
#ifdef WIN32
	Indy::WindowInputWin32* win32Window = (Indy::WindowInputWin32*)window;
	Indy::GLContextWin32* win32GLContext = new Indy::GLContextWin32( openGLInfo);
	// pixel format descriptor is defaulted in the class (can be manually inserted)
	win32GLContext->Create( win32Window->GetDeviceContext(), NULL);
	glContext = win32GLContext;
#endif
	glContext->Bind();
	
	initializeGlew();

	Indy::Timer gameClock;
	gameClock.Start();

	Indy::Game game;

	// start of loop
	bool hasStopped = false;
	while( !hasStopped)
	{
		gameClock.Stop();
		const double frameTimeInSeconds = gameClock.GetSeconds();
		gameClock.Start();

		window->SwapBuffer();

		// QQQ
		if( keyboard.IsKeyPressed(Indy::KeyboardKeys::ESCAPE))
			hasStopped = true;
		
		// process window events (on windows input gets handled here)
		window->ProcessWindowEvents();

		if( window->IsQuitRequested())
			hasStopped = true;

		game.UpdateFrame( frameTimeInSeconds);

		game.DrawFrame( frameTimeInSeconds);

		// check GL error
		glContext->HasGLError();
	}

	// deinitialize
	glContext->UnBind();
	delete glContext;
	glContext = NULL;

	window->Destroy();
	delete window;
	window = NULL;

	return 0;
}