// Juul Joosten 2013

#pragma once

#include <IndyCore/Utilities/NonCopyable.h>

namespace Indy
{
	class Keyboard;
	class GLContext;
	class Window;
	class GameBlueprint : public NonCopyable
	{
	public:
		virtual void Create(GLContext* const openGLContext, Window* const window, Keyboard* const keyboard) = 0;
		virtual void Destroy(void) = 0;
		virtual void UpdateFrame(const double deltaTimeSec) = 0;
		virtual void DrawFrame(const double deltaTimeSec) = 0;
	};

	class _imp_game;
	class Game : public NonCopyable
	{

	public:
		Game( GLContext* const openGLContext, Window* const window, Keyboard* const keyboard);
		~Game( void);

		void UpdateFrame( const double deltaTimeSec);
		void DrawFrame  ( const double deltaTimeSec);

	private:
		_imp_game* m_game;
	};
}