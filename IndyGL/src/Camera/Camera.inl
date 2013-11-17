// Juul Joosten 2013

float Indy::Camera::GetYawInDegrees( void) const
{
	return m_yawInDegrees;
}

float Indy::Camera::GetPitchInDegrees( void) const
{
	return m_pitchInDegrees;
}

float Indy::Camera::GetRollInDegrees( void) const
{
	return m_rollInDegrees;
}

void Indy::Camera::GetPosition( float& xOUT, float& yOUT, float& zOUT) const
{
	xOUT = m_position[0];
	yOUT = m_position[1];
	zOUT = m_position[2];
}

void Indy::Camera::GetUp( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 1
	xOUT = m_viewMatrix[1];
	yOUT = m_viewMatrix[5];
	zOUT = m_viewMatrix[9];
}

void Indy::Camera::GetRight( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 0
	xOUT = m_viewMatrix[0];
	yOUT = m_viewMatrix[4];
	zOUT = m_viewMatrix[8];
}

void Indy::Camera::GetForward( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 2
	xOUT = m_viewMatrix[ 2];
	yOUT = m_viewMatrix[ 6];
	zOUT = m_viewMatrix[10];  
}