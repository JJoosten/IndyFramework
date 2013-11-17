// Juul Joosten 2013

#pragma once

#include <IndyCore/Math/Matrix4.h>

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

		inline float GetYawInDegrees( void) const;
		inline float GetPitchInDegrees( void) const;
		inline float GetRollInDegrees( void) const;

		inline void GetPosition( float& xOUT, float& yOUT, float& zOUT) const;

		inline void GetUp( float& xOUT, float& yOUT, float& zOUT) const;
		inline void GetRight( float& xOUT, float& yOUT, float& zOUT) const;
		inline void GetForward( float& xOUT, float& yOUT, float& zOUT) const;

	private:
		void computeViewMatrix( void);

	protected:
		Matrix4f	m_projectionMatrix;
		Matrix4f	m_viewMatrix;
		Vector3f	m_position;
		float		m_yawInDegrees;
		float		m_pitchInDegrees;
		float		m_rollInDegrees;
	};

	#include "Camera.inl"
}