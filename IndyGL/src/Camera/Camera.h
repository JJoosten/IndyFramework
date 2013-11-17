// Juul Joosten 2013

#pragma once

namespace Indy
{
	class Camera
	{

	public:
		Camera( void);
		~Camera( void);

		void Init( const float fov, 
				   const float aspectRatio, 
				   const float nearPlane, 
				   const float farPlane);

		void Init( const float fov, 
				   const float aspectRatio, 
				   const float nearPlane, 
				   const float farPlane, 
				   const float* const position);

		void Init( const float left, 
				   const float right, 
				   const float bottom, 
				   const float top, 
				   const float near, 
				   const float far);

		void Init( const float left, 
				   const float right, 
				   const float bottom, 
				   const float top, 
				   const float near, 
				   const float far, 
				   const float* const position);


		void SetPerspectiveProjection( const float fov, 
									   const float aspectRatio, 
									   const float nearPlane, 
									   const float farPlane);

		void SetPerspectiveProjection( const float* const projection);

		void SetOrthographicProjection( const float left, 
										const float right, 
										const float bottom, 
										const float top, 
										const float near, 
										const float far);

		void SetOrthographicProjection( const float* const projection);

		void SetPosition( const float* const position);

		void LookAt( const float* const eyePosition, 
					 const float* const targetPosition, 
					 const float* const worldUpNormal);

		void MoveOverRelativeX( const float distance);
		void MoveOverRelativeY( const float distance);
		void MoveOverRelativeZ( const float distance);

		void Yaw( const float degrees);
		void Pitch( const float degrees);
		void Roll( const float degrees);

		float GetYawInDegrees( void) const;
		float GetPitchInDegrees( void) const;
		float GetRollInDegrees( void) const;

		void GetPosition( float& xOUT, float& yOUT, float& zOUT) const;

		void GetUp( float& xOUT, float& yOUT, float& zOUT) const;
		void GetRight( float& xOUT, float& yOUT, float& zOUT) const;
		void GetForward( float& xOUT, float& yOUT, float& zOUT) const;

	private:
		void computeViewMatrix( void);

	protected:
		float m_projectionMatrix[16];
		float m_viewMatrix[16];
		float m_position[3];
		float m_yawInDegrees;
		float m_pitchInDegrees;
		float m_rollInDegrees;
	};
}

#include "Camera.inl"