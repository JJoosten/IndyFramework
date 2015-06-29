// Juul Joosten 2013

#pragma once

#include "Vector3.h"

#ifndef M_PI
	#define M_PI	3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798
#endif

#ifndef DEGREES_TO_RADIANS
	#define DEGREES_TO_RADIANS(D)	(D * M_PI / 180.0)
#endif

#ifndef RADIANS_TO_DEGREES
#define RADIANS_TO_DEGREES(R)	(R * 57.2957795131)
#endif


namespace Indy
{
	template<typename T>
	class Matrix4
	{
		
	public:
		Matrix4()
		{
			this->Identity();
		}

		Matrix4(const T* matrix)
		{
			memcpy( Data, matrix, sizeof(T) * 16 );
		}

		Matrix4(const Matrix4<T>& cpyMatrix)
		{
			memcpy( Data, cpyMatrix.GetReadOnlyData(), sizeof(T) * 16 );
		}

		~Matrix4()
		{
		}

		const Matrix4<T>& operator =(const Matrix4<T>& matB)
		{
			memcpy( Data, matB.GetReadOnlyData(), sizeof(T) * 16);
			return *this;
		}


		inline void Identity()
		{
			// set identity matrix
			memset( Data, 0, sizeof(T) * 16 );
			C1R1 = 1;	//[1][0][0][0]
			C2R2 = 1;	//[0][1][0][0]
			C3R3 = 1;	//[0][0][1][0]
			C4R4 = 1;	//[0][0][0][1]
		}

		inline void Zero()
		{
			memset( Data, 0, sizeof(T) * 16);
		}


		Matrix4<T> operator +(const Matrix4<T>& matB) const
		{
			return Matrix4<T>(Data) += matB;
		}

		Matrix4<T> operator -(const Matrix4<T>& matB) const
		{
			return Matrix4<T>(Data) -= matB;
		}

		Matrix4<T> operator *(const Matrix4<T>& matB) const
		{
			Matrix4<T> matrix = Matrix4<T>(Data);
			return matrix *= matB;
		}


		inline Matrix4<T>& operator +=(const Matrix4<T>& matB)
		{
			for (int i = 0; i < 16; ++i)
				Data[i] += matB.GetReadOnlyData()[i];

			return *this;
		}

		inline Matrix4<T>& operator -=(const Matrix4<T>& matB)
		{
			for (int i = 0; i < 16; ++i)
				Data[i] -= matB.GetReadOnlyData()[i];

			return *this;
		}

		inline Matrix4<T>& operator *=(const Matrix4<T>& matB)
		{
			Matrix4<T> matA(*this);
			Matrix4<T> bTransposed = Matrix4<T>(matB);//.Transpose();
			
			const T* const originalData = matA.GetReadOnlyData();
			const T* const tranposedData = bTransposed.GetReadOnlyData();
			for( unsigned int j = 0; j < 4; ++j)
			{
				for( unsigned int i = 0; i < 4; ++i)
					Data[j + i * 4] = originalData[j + 0 * 4] * tranposedData[i * 4 + 0] + 
										originalData[j + 1 * 4] * tranposedData[i * 4 + 1] +
										originalData[j + 2 * 4] * tranposedData[i * 4 + 2] +
										originalData[j + 3 * 4] * tranposedData[i * 4 + 3];

			}

			return (*this);
		}


		inline Matrix4<T>& Transpose()
		{
			T tempVal = C2R1;
			C2R1 = C1R2;				
			C1R2 = tempVal;

			tempVal = C3R1;
			C3R1 = C1R3;
			C1R3 = tempVal;

			tempVal = C4R1;
			C4R1 = C1R4;
			C1R4 = tempVal;

			tempVal = C4R2;
			C4R2 = C2R4;
			C2R4 = tempVal;

			tempVal = C4R3;
			C4R3 = C3R4;
			C3R4 = tempVal;

			return *this;
		}

		inline Matrix4<T>& Inverse( void)
		{
			// calculate adjucate matrix
			T determinant3x3[16];

			determinant3x3[0] = Data[5]  * Data[10] * Data[15] - 
						Data[5]  * Data[11] * Data[14] - 
						Data[9]  * Data[6]  * Data[15] + 
						Data[9]  * Data[7]  * Data[14] +
						Data[13] * Data[6]  * Data[11] - 
						Data[13] * Data[7]  * Data[10];

			determinant3x3[4] = -Data[4]  * Data[10] * Data[15] + 
						Data[4]  * Data[11] * Data[14] + 
						Data[8]  * Data[6]  * Data[15] - 
						Data[8]  * Data[7]  * Data[14] - 
						Data[12] * Data[6]  * Data[11] + 
						Data[12] * Data[7]  * Data[10];

			determinant3x3[8] = Data[4]  * Data[9] * Data[15] - 
						Data[4]  * Data[11] * Data[13] - 
						Data[8]  * Data[5] * Data[15] + 
						Data[8]  * Data[7] * Data[13] + 
						Data[12] * Data[5] * Data[11] - 
						Data[12] * Data[7] * Data[9];

			determinant3x3[12] = -Data[4]  * Data[9] * Data[14] + 
						Data[4]  * Data[10] * Data[13] +
						Data[8]  * Data[5] * Data[14] - 
						Data[8]  * Data[6] * Data[13] - 
						Data[12] * Data[5] * Data[10] + 
						Data[12] * Data[6] * Data[9];

			determinant3x3[1] = -Data[1]  * Data[10] * Data[15] + 
						Data[1]  * Data[11] * Data[14] + 
						Data[9]  * Data[2] * Data[15] - 
						Data[9]  * Data[3] * Data[14] - 
						Data[13] * Data[2] * Data[11] + 
						Data[13] * Data[3] * Data[10];

			determinant3x3[5] = Data[0]  * Data[10] * Data[15] - 
						Data[0]  * Data[11] * Data[14] - 
						Data[8]  * Data[2] * Data[15] + 
						Data[8]  * Data[3] * Data[14] + 
						Data[12] * Data[2] * Data[11] - 
						Data[12] * Data[3] * Data[10];

			determinant3x3[9] = -Data[0]  * Data[9] * Data[15] + 
						Data[0]  * Data[11] * Data[13] + 
						Data[8]  * Data[1] * Data[15] - 
						Data[8]  * Data[3] * Data[13] - 
						Data[12] * Data[1] * Data[11] + 
						Data[12] * Data[3] * Data[9];

			determinant3x3[13] = Data[0]  * Data[9] * Data[14] - 
						Data[0]  * Data[10] * Data[13] - 
						Data[8]  * Data[1] * Data[14] + 
						Data[8]  * Data[2] * Data[13] + 
						Data[12] * Data[1] * Data[10] - 
						Data[12] * Data[2] * Data[9];

			determinant3x3[2] = Data[1]  * Data[6] * Data[15] - 
						Data[1]  * Data[7] * Data[14] - 
						Data[5]  * Data[2] * Data[15] + 
						Data[5]  * Data[3] * Data[14] + 
						Data[13] * Data[2] * Data[7] - 
						Data[13] * Data[3] * Data[6];

			determinant3x3[6] = -Data[0]  * Data[6] * Data[15] + 
						Data[0]  * Data[7] * Data[14] + 
						Data[4]  * Data[2] * Data[15] - 
						Data[4]  * Data[3] * Data[14] - 
						Data[12] * Data[2] * Data[7] + 
						Data[12] * Data[3] * Data[6];

			determinant3x3[10] = Data[0]  * Data[5] * Data[15] - 
						Data[0]  * Data[7] * Data[13] - 
						Data[4]  * Data[1] * Data[15] + 
						Data[4]  * Data[3] * Data[13] + 
						Data[12] * Data[1] * Data[7] - 
						Data[12] * Data[3] * Data[5];

			determinant3x3[14] = -Data[0]  * Data[5] * Data[14] + 
						Data[0]  * Data[6] * Data[13] + 
						Data[4]  * Data[1] * Data[14] - 
						Data[4]  * Data[2] * Data[13] - 
						Data[12] * Data[1] * Data[6] + 
						Data[12] * Data[2] * Data[5];

			determinant3x3[3] = -Data[1] * Data[6] * Data[11] + 
						Data[1] * Data[7] * Data[10] + 
						Data[5] * Data[2] * Data[11] - 
						Data[5] * Data[3] * Data[10] - 
						Data[9] * Data[2] * Data[7] + 
						Data[9] * Data[3] * Data[6];

			determinant3x3[7] = Data[0] * Data[6] * Data[11] - 
						Data[0] * Data[7] * Data[10] - 
						Data[4] * Data[2] * Data[11] + 
						Data[4] * Data[3] * Data[10] + 
						Data[8] * Data[2] * Data[7] - 
						Data[8] * Data[3] * Data[6];

			determinant3x3[11] = -Data[0] * Data[5] * Data[11] + 
						Data[0] * Data[7] * Data[9] + 
						Data[4] * Data[1] * Data[11] - 
						Data[4] * Data[3] * Data[9] - 
						Data[8] * Data[1] * Data[7] + 
						Data[8] * Data[3] * Data[5];

			determinant3x3[15] = Data[0] * Data[5] * Data[10] - 
						Data[0] * Data[6] * Data[9] - 
						Data[4] * Data[1] * Data[10] + 
						Data[4] * Data[2] * Data[9] + 
						Data[8] * Data[1] * Data[6] - 
						Data[8] * Data[2] * Data[5];

			T determinant = Data[0] * determinant3x3[0] + Data[1] * determinant3x3[4] + Data[2] * determinant3x3[8] + Data[3] * determinant3x3[12];

			if (determinant == 0)
				return *this;

			determinant = 1.0f / determinant;

			// inverse = 1 / determinant * (adjugateMatrix)
			for ( unsigned int i = 0; i < 16; ++i)
				Data[i] = determinant3x3[i] * determinant;

			return *this;
		}


		inline void Translate(const Vector3<T>& pos)
		{
			Matrix4<T> mTranslate;
			mTranslate.SetTranslation(pos);
			(*this) *= mTranslate;
		}

		inline void Translate(T* pos)
		{
			Matrix4<T> mTranslate;
			mTranslate.SetTranslation(pos);
			(*this) *= mTranslate;
		}

		inline void Translate(T x, T y, T z)
		{
			Matrix4<T> mTranslate;
			mTranslate.SetTranslation(x, y, z);
			(*this) *= mTranslate;
		}

		inline void SetTranslation(const Vector3<T>& pos)
		{
			C4R1 = pos.X;
			C4R2 = pos.Y;
			C4R3 = pos.Z;
		}

		inline void SetTranslation(T* pos)
		{
			C4R1 = pos[0];
			C4R2 = pos[1];
			C4R3 = pos[2];
		}
		
		inline void SetTranslation(T x, T y, T z)
		{
			C4R1 = x;
			C4R2 = y;
			C4R3 = z;
		}


		inline void Scale(T scale)
		{
			C1R1 *= scale;
			C2R2 *= scale;
			C3R3 *= scale;
		}

		inline void NonUniformScale(T* vec)
		{
			C1R1 *= vec[0];
			C2R2 *= vec[1];
			C3R3 *= vec[2];
		}


		Matrix4<T>& RotateX(T degrees)
		{
			const T radians = (T)DEGREES_TO_RADIANS( degrees);

			C2R2 = cos( radians); 
			C2R3 = -sin( radians);
			C3R2 = sin( radians); 
			C3R3 = cos( radians);

			return *this;
		} 

		Matrix4<T>& RotateY(T degrees)
		{
			const T radians = (T)DEGREES_TO_RADIANS( degrees);

			C1R1 = cos( radians); 
			C1R3 = -sin( radians);
			C3R1 = sin( radians); 
			C3R3 = cos( radians);

			return *this;
		} 

		Matrix4<T>& RotateZ(T degrees)
		{
			const T radians = (T)DEGREES_TO_RADIANS( degrees);

			C1R1 = cos( radians); 
			C1R2 = -sin( radians);
			C2R1 = sin( radians); 
			C2R2 = cos( radians);

			return *this;
		} 

		inline Matrix4<T>& RotateAroundAxis( const T normalX, 
										   const T normalY, 
										   const T normalZ, 
										   const T degrees)
		{
			const T rads = (T)DEGREES_TO_RADIANS(degrees);
			const T cosine = cos(rads);
			const T oneMinCos = 1.0f - cosine;
			const T sine = sin(rads);

			const T xz = normalX * normalZ * oneMinCos;
			const T xy = normalX * normalY * oneMinCos;
			const T yz = normalY * normalZ * oneMinCos;

			const T normalZSine = normalZ * sine;
			const T normalYSine = normalY * sine;
			const T normalXSine = normalX * sine;


			// column 1
			Data[ 0] = (normalX * normalX) * oneMinCos + cosine;
			Data[ 1] = xy + normalZSine;
			Data[ 2] = xz - normalYSine;

			// column 2
			Data[ 4] = xy - normalZSine;
			Data[ 5] = (normalY * normalY) * oneMinCos + cosine;
			Data[ 6] = yz + normalXSine;

			// column 3
			Data[ 8] = xz + normalYSine;
			Data[ 9] = yz - normalXSine;
			Data[10] = (normalZ * normalZ) * oneMinCos + cosine;

			return *this;
		}	


		void CreateLookAt( const Vector3<T>& pos, const Vector3<T>& target, const  Vector3<T>& up)
		{
			memset( Data, 0, sizeof(T) * 16 );

			Vector3<T> direction = (target - pos).Normalize();
			Vector3<T> right = Cross(direction, up).Normalize();
			Vector3<T> localUp = Cross(right, direction);

			C1R1 = right.X;
			C2R1 = right.Y;
			C3R1 = right.Z;
			C1R2 = localUp.X;
			C2R2 = localUp.Y;
			C3R2 = localUp.Z;
			C1R3 = -direction.X;
			C2R3 = -direction.Y;
			C3R3 = -direction.Z;
			C4R4 = 1;

			// translate to position
			Translate(-Vector3<T>(pos));
		}


		/* --- Getters & Setters --- */
		T& operator[]( const unsigned int index) { return Data[index]; }

		const T* const GetReadOnlyData( void) const { return Data; }

		Vector3<T> GetXYZColumn( const unsigned int index) const
		{
			return Vector3<T>( Data[index * 4 + 0], Data[index * 4 + 1], Data[index * 4 + 2] );
		}

		Vector3<T> GetXYZRow( const unsigned int index) const
		{
			return Vector3<T>( Data[index + 0], Data[index + 4], Data[index + 8] );
		}

	private:
		union
		{
			struct { T C1R1, C1R2, C1R3, C1R4, C2R1, C2R2, C2R3, C2R4, C3R1, C3R2, C3R3, C3R4, C4R1, C4R2, C4R3, C4R4; };
			struct { T Data[16]; };
		};
	};

	
	typedef Matrix4<float>				Matrix4f;
	typedef Matrix4<double>				Matrix4d;	
	typedef Matrix4<unsigned int>		Matrix4ui;
	typedef Matrix4<int>				Matrix4i;	
	typedef Matrix4<short>				Matrix4s;	
	typedef Matrix4<unsigned short>		Matrix4us;	
	typedef Matrix4<char>				Matrix4c;	
	typedef Matrix4<unsigned char>		Matrix4uc;	

	template< typename T>
	inline Matrix4<T> CreateFromYawPitchRoll( T degreesYaw, T degreesPitch, T degreesRoll)
	{
		Matrix4<T> matrix = Matrix4<T>();

		Matrix4<T> rotateY;
		rotateY.RotateY(degreesPitch);

		Matrix4<T> rotateX;
		rotateX.RotateX(degreesYaw);
		
		Matrix4<T> rotateZ;
		rotateZ.RotateZ(degreesRoll);

		return rotateY * rotateX * rotateZ;
	};
	
	template< typename T>
	inline Matrix4<T> CreateLookatMatrix( const Vector3f& pos, const Vector3f& target, const Vector3f& up )
	{
		Matrix4<T> matrix = Matrix4<T>();

		matrix.CreateLookAt( pos, target, up);
			
		return matrix;
	};
	
	template< typename T>
	inline Matrix4<T> CreatePerspectiveMatrix(T fov, T aspect, T znear, T zfar)
	{
		T data[16];
		memset( data, 0, sizeof(T) * 16);

		const T f = 1.0f / tanf( DEGREES_TO_RADIANS(fov) / 2.0f);

		data[ 0] = f / aspect;
		data[ 5] = f;
		data[10] = (zfar + znear) / (znear - zfar);
		data[11] = -1;
		data[14] = 2 * zfar * znear / (znear - zfar);

		return Matrix4<T>(data);
	};
	
	template< typename T>
	inline static Matrix4<T> CreateOrthographicMatrix( const T xLeft, 
													 const T xRight,
													 const T yBottom,
													 const T yTop,
													 const T zNear,
													 const T zFar)
	{
		T data[16];
		memset( data, 0, sizeof(T) * 16);

		data[ 0] = 2.0f / (xRight - xLeft);
		data[ 5] = 2.0f / (yTop - yBottom);
		data[10] = 1.0f / (zFar - zNear);
		data[12] = -((xRight + xLeft) / (xRight - xLeft));
		data[13] = -((yTop + yBottom) / (yTop - yBottom));
		data[14] = -(zNear / (zFar - zNear));
		data[15] = 1.0f;

		return Matrix4<T>(data);
	};

}