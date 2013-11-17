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
		m_projectionMatrix = CreatePerspectiveMatrix( fov, aspectRatio, nearPlane, farPlane);
	}

	void Camera::SetPerspectiveProjection( const float* const projection)
	{
		memcpy( &m_projectionMatrix, projection, sizeof(float) * 16);
	}

	void Camera::SetOrthographicProjection( const float left, 
											const float right, 
											const float bottom, 
											const float top, 
											const float near, 
											const float far)
	{
		m_projectionMatrix = CreateOrthographicMatrix( left, right, bottom, top, near, far);
	}

	void Camera::SetOrthographicProjection( const float* const projection)
	{
		memcpy( &m_projectionMatrix, projection, sizeof(float) * 16);
	}

	
	void Camera::SetPosition( const float* const position)
	{
		m_position.X = position[0];
		m_position.Y = position[1];
		m_position.Z = position[2];

		this->computeViewMatrix();
	}
	
	void Camera::LookAt( const float* const eyePositionXYZ, 
						 const float* const targetPositionXYZ, 
						 const float* const worldUpNormalXYZ)
	{
		m_position = Vector3f(eyePositionXYZ);
		m_viewMatrix = CreateLookatMatrix<float>( m_position, Vector3f(targetPositionXYZ), Vector3f(worldUpNormalXYZ));
	}


	void Camera::MoveOverRelativeX( const float distance)
	{
		float xRelativeX = 0;
		float yRelativeX = 0; 
		float zRelativeX = 0;
		this->GetForward( xRelativeX, yRelativeX, zRelativeX);

		m_position.X += xRelativeX * distance;
		m_position.Y += yRelativeX * distance;
		m_position.Z += zRelativeX * distance;

		this->computeViewMatrix();
	}

	void Camera::MoveOverRelativeY( const float distance)
	{
		float xRelativeY = 0;
		float yRelativeY = 0; 
		float zRelativeY = 0;
		this->GetForward( xRelativeY, yRelativeY, zRelativeY);

		m_position.X += xRelativeY * distance;
		m_position.Y += yRelativeY * distance;
		m_position.Z += zRelativeY * distance;

		this->computeViewMatrix();
	}

	void Camera::MoveOverRelativeZ( const float distance)
	{
		float xRelativeZ = 0;
		float yRelativeZ = 0; 
		float zRelativeZ = 0;
		this->GetForward( xRelativeZ, yRelativeZ, zRelativeZ);

		m_position.X += xRelativeZ * distance;
		m_position.Y += yRelativeZ * distance;
		m_position.Z += zRelativeZ * distance;

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
		Matrix4f rotMatrix;
		rotMatrix *= Matrix4f().RotateX(m_pitchInDegrees);
		rotMatrix *= Matrix4f().RotateY(m_yawInDegrees);
		rotMatrix *= Matrix4f().RotateZ(m_rollInDegrees);
	
		LookAt(m_position.XYZ, (m_position + rotMatrix.GetXYZRow(2)).XYZ, rotMatrix.GetXYZRow(1).XYZ);
	}
}