// Juul Joosten 2013

void Indy::Keyboard::SetKeyDown( const Indy::KeyboardKeys::KeyboardKey key)
{
	SetKeyPressed( key);
	m_keysDown[key] = true;
}

void Indy::Keyboard::SetKeyPressed( const Indy::KeyboardKeys::KeyboardKey key)
{
	m_keysPressed[key] = true;
}

void Indy::Keyboard::SetKeyUp( const Indy::KeyboardKeys::KeyboardKey key)
{
	m_keysDown[key] = false;
}


bool Indy::Keyboard::IsKeyDown( const Indy::KeyboardKeys::KeyboardKey key)
{
	return m_keysDown[key];
}

bool Indy::Keyboard::IsAnyKeyDown( void)
{
	for( unsigned int i = 0; i < NUM_KEYBOARD_KEYS; ++i)
		if( m_keysDown[i] == true)
			return true;

	return false;
}			


bool Indy::Keyboard::IsKeyUp( const Indy::KeyboardKeys::KeyboardKey key)
{
	return !IsKeyDown( key);
}


bool Indy::Keyboard::IsKeyPressed( const Indy::KeyboardKeys::KeyboardKey key)
{
	return m_keysPressed[key];
}

bool Indy::Keyboard::IsAnyKeyPressed( void)
{
	for( unsigned int i = 0; i < NUM_KEYBOARD_KEYS; ++i)
		if( m_keysPressed[i] == true)
			return true;

	return false;
}