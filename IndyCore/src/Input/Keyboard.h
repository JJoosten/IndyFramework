// Juul Joosten 2013

#pragma once

#include "KeyboardKeys.h"
#include "../Utilities/Singleton.h"

#define NUM_KEYBOARD_KEYS 256

namespace Indy
{
	class Keyboard
	{

	public:
		Keyboard( void);
		~Keyboard( void);


		inline void SetKeyDown( const KeyboardKeys::KeyboardKey key);

		inline void SetKeyPressed( const KeyboardKeys::KeyboardKey key);

		inline void SetKeyUp( const KeyboardKeys::KeyboardKey key);

		inline bool IsKeyDown( const KeyboardKeys::KeyboardKey key) const;
		inline bool IsAnyKeyDown( void) const;

		inline bool IsKeyUp( const KeyboardKeys::KeyboardKey key) const;

		inline bool IsKeyPressed( const KeyboardKeys::KeyboardKey key) const;
		inline bool IsAnyKeyPressed( void) const;
		

	private:
		bool m_keysDown[NUM_KEYBOARD_KEYS];
		bool m_keysPressed[NUM_KEYBOARD_KEYS];

	};

	#include "Keyboard.inl"
};