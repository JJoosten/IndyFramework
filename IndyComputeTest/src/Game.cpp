// Juul Joosten 2013

#include "Game.h"

#include "Paths.h"

#include <IndyCore/CoreDefines.h>
#include <IndyCore/Input/Keyboard.h>
#include <IndyGL/Window/Window.h>
#include <IndyGL/Context/GLContext.h>

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// scenes
#include "GameOceanSim.inl" // ocean FFT Compute
//#include "GameRaytracing.inl" // simple raytracing Compute
//#include "GameReference.inl" // reference game implementation

namespace Indy
{
	Game::Game(GLContext* const openGLContext, Window* const window, Keyboard* const keyboard)
		:
	m_game(new _imp_game())
	{
		m_game->Create(openGLContext, window, keyboard);
	}

	Game::~Game( void)
	{
		m_game->Destroy();
		delete m_game;
		m_game = NULL;
	}


	void Game::UpdateFrame( const double deltaTimeSec)
	{
		m_game->UpdateFrame(deltaTimeSec);
	}

	void Game::DrawFrame( const double deltaTimeSec)
	{
		m_game->DrawFrame(deltaTimeSec);
	}
}