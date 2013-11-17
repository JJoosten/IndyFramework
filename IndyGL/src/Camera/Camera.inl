// Juul Joosten 2013

float Indy::Camera::GetYawInDegrees( void) const
{
	return m_yawInDegrees;
};

float Indy::Camera::GetPitchInDegrees( void) const
{
	return m_pitchInDegrees;
};

float Indy::Camera::GetRollInDegrees( void) const
{
	return m_rollInDegrees;
};

void Indy::Camera::GetPosition( float& xOUT, float& yOUT, float& zOUT) const
{
	xOUT = m_position.X;
	yOUT = m_position.Y;
	zOUT = m_position.Z;
};

void Indy::Camera::GetUp( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 1
	Vector3f row1 = m_viewMatrix.GetXYZRow(1);
	xOUT = row1.X;
	yOUT = row1.Y;
	zOUT = row1.Z;
};

void Indy::Camera::GetRight( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 0
	Vector3f row0 = m_viewMatrix.GetXYZRow(0);
	xOUT = row0.X;
	yOUT = row0.Y;
	zOUT = row0.Z;
};

void Indy::Camera::GetForward( float& xOUT, float& yOUT, float& zOUT) const
{
	// get row 2
	Vector3f row2 = m_viewMatrix.GetXYZRow(2);
	xOUT = row2.X;
	yOUT = row2.Y;
	zOUT = row2.Z; 
};