// Juul Joosten 2013

#include "Camera.h"
#include <memory>

namespace Indy
{
	Camera::Camera( void)
		:
	m_projectionMatrix()
	,m_viewMatrix()
	,m_position()
	,m_yawInDegrees()
	,m_pitchInDegrees()
	,m_rollInDegrees()
	{
		// identity
		m_projectionMatrix[ 0] = 1.0f;
		m_projectionMatrix[ 5] = 1.0f;
		m_projectionMatrix[10] = 1.0f;
		m_projectionMatrix[15] = 1.0f;

		// identity
		m_viewMatrix[ 0] = 1.0f;
		m_viewMatrix[ 5] = 1.0f;
		m_viewMatrix[10] = 1.0f;
		m_viewMatrix[15] = 1.0f;
	}

	Camera::~Camera( void)
	{

	}


	void Camera::Init( const float fov, 
					   const float aspectRatio, 
					   const float nearPlane, 
					   const float farPlane)
	{
		this->SetPerspectiveProjection( fov, aspectRatio, nearPlane, farPlane);
	}

	void Camera::Init( const float fov, 
					   const float aspectRatio, 
					   const float nearPlane, 
					   const float farPlane, 
					   const float* const position)
	{
		this->SetPerspectiveProjection( fov, aspectRatio, nearPlane, farPlane);

		this->SetPosition( position);
	}

	void Camera::Init( const float left, 
					   const float right, 
					   const float bottom, 
					   const float top, 
					   const float near, 
					   const float far)
	{
		this->SetOrthographicProjection( left, right, bottom, top, near, far);
	}

	void Camera::Init( const float left, 
					   const float right, 
					   const float bottom, 
					   const float top, 
					   const float near, 
					   const float far, 
					   const float* const position)
	{
		this->SetOrthographicProjection( left, right, bottom, top, near, far);

		this->SetPosition( position);
	}


	void Camera::SetPerspectiveProjection( const float fov, 
										   const float aspectRatio, 
										   const float nearPlane, 
										   const float farPlane)
	{
		
	}

	void Camera::SetPerspectiveProjection( const float* const projection)
	{
		memcpy( m_projectionMatrix, projection, sizeof(float) * 16);
	}

	void Camera::SetOrthographicProjection( const float left, 
											const float right, 
											const float bottom, 
											const float top, 
											const float near, 
											const float far)
	{

	}

	void Camera::SetOrthographicProjection( const float* const projection)
	{
		memcpy( m_projectionMatrix, projection, sizeof(float) * 16);
	}

	
	void Camera::SetPosition( const float* const position)
	{
		m_position[0] = position[0];
		m_position[1] = position[1];
		m_position[2] = position[2];

		this->computeViewMatrix();
	}
	
	void Camera::LookAt( const float* const eyePositionXYZ, 
						 const float* const targetPositionXYZ, 
						 const float* const worldUpNormalXYZ)
	{

	}


	void Camera::MoveOverRelativeX( const float distance)
	{
		float xRelativeX = 0;
		float yRelativeX = 0; 
		float zRelativeX = 0;
		this->GetForward( xRelativeX, yRelativeX, zRelativeX);

		m_position[0] += xRelativeX * distance;
		m_position[1] += yRelativeX * distance;
		m_position[2] += zRelativeX * distance;

		this->computeViewMatrix();
	}

	void Camera::MoveOverRelativeY( const float distance)
	{
		float xRelativeY = 0;
		float yRelativeY = 0; 
		float zRelativeY = 0;
		this->GetForward( xRelativeY, yRelativeY, zRelativeY);

		m_position[0] += xRelativeY * distance;
		m_position[1] += yRelativeY * distance;
		m_position[2] += zRelativeY * distance;

		this->computeViewMatrix();
	}

	void Camera::MoveOverRelativeZ( const float distance)
	{
		float xRelativeZ = 0;
		float yRelativeZ = 0; 
		float zRelativeZ = 0;
		this->GetForward( xRelativeZ, yRelativeZ, zRelativeZ);

		m_position[0] += xRelativeZ * distance;
		m_position[1] += yRelativeZ * distance;
		m_position[2] += zRelativeZ * distance;

		this->computeViewMatrix();
	}


	void Camera::Yaw( const float degrees)
	{
		m_yawInDegrees += degrees;
		this->computeViewMatrix();
	}

	void Camera::Pitch( const float degrees)
	{
		m_pitchInDegrees += degrees;
		this->computeViewMatrix();
	}

	void Camera::Roll( const float degrees)
	{
		m_rollInDegrees += degrees;
		this->computeViewMatrix();
	}

	
	void Camera::computeViewMatrix( void)
	{

	}
}