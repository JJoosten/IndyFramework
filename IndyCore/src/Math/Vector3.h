// Juul Joosten 2013

#pragma once

#include <math.h>
#include <memory>

namespace Indy
{
	template< typename T>
	class Vector3
	{

	public:
		// ctors, copy ctors and assignment operators
		Vector3( void)
			:
		 X(0)
		,Y(0)
		,Z(0)
		{ }

		Vector3( T value)
			:
		 X(value)
		,Y(value)
		,Z(value)
		{ }

		Vector3( T x, T y, T z)
			:
		 X(x)
		,Y(y)
		,Z(z)
		{ }

		Vector3(const Vector3<T>& cpyVec)
			:
		 X(cpyVec.X)
		,Y(cpyVec.Y)
		,Z(cpyVec.Z)
		{ }

		Vector3(const T* xyz)
		{
			memcpy(XYZ, xyz, sizeof(T) * 3);
		}


		Vector3<T>& operator =(const Vector3<T>& vecB)
		{
			memcpy(XYZ, vecB.XYZ, sizeof(T) * 3);
			return *this;
		}

		bool operator !=(const Vector3<T>& vecB) 
		{
			return X != vecB.X || Y != vecB.Y || Z != vecB.Z;
		}


		Vector3<T> operator -() 
		{  
			return Vector3f(-X,-Y,-Z); 
		}

		Vector3<T> operator +(const Vector3<T>& vecB)
		{
			return Vector3f(X + vecB.X, Y + vecB.Y, Z + vecB.Z);
		}

		Vector3<T> operator -(const Vector3<T>& vecB)
		{
			return Vector3f(X - vecB.X, Y - vecB.Y, Z - vecB.Z);
		}

		Vector3<T> operator *(const Vector3<T>& vecB)
		{
			return Vector3f(X * vecB.X, Y * vecB.Y, Z * vecB.Z);
		}

		Vector3<T> operator *(const T value)
		{
			return Vector3f(X * value, Y * value, Z * value);
		}

		Vector3<T> operator /( T value)
		{
			value =  1.0f / value;
			return Vector3f(X * value, Y * value, Z * value);
		}

		Vector3<T>& operator +=(const Vector3<T>& vecB)
		{
			X += vecB.X;
			Y += vecB.Y;
			Z += vecB.Z;
			return *this;
		}

		Vector3<T>& operator -=(const Vector3<T>& vecB)
		{
			X -= vecB.X;
			Y -= vecB.Y;
			Z -= vecB.Z;
			return *this;
		}

		Vector3<T>& operator *=( const Vector3<T>& vecB)
		{
			X *= vecB.X;
			Y *= vecB.Y;
			Z *= vecB.Z;

			return *this;
		}

		Vector3<T>& operator *=( const T value)
		{
			X *= value;
			Y *= value;
			Z *= value;
			return *this;
		}

		Vector3<T>& operator /=( T value)
		{
			value = 1.0f / value;
			X *= value;
			Y *= value;
			Z *= value;
			return *this;
		}

		void Zero() 
		{ 
			X, Y, Z = 0; 
		}

		Vector3<T>& Normalize()
		{
			T sqrtMag = GetLengthSquared(*this);

			if (sqrtMag > 0.0f)
			{
				T oneOverMag = 1.0f / sqrt(sqrtMag);
				X *= oneOverMag;
				Y *= oneOverMag;
				Z *= oneOverMag;
			}

			return *this;
		}


	public:
		union
		{
			struct 
			{
				T X;
				T Y;
				T Z;
			};
			struct 
			{
				T R;
				T G;
				T B;
			};
			struct
			{
				T XYZ[3];
			};
			struct
			{
				T RGB[3];
			};
		};

	};

	typedef Vector3<float>				Vector3f;
	typedef Vector3<double>				Vector3d;	
	typedef Vector3<unsigned int>		Vector3ui;
	typedef Vector3<int>				Vector3i;	
	typedef Vector3<short>				Vector3s;	
	typedef Vector3<unsigned short>		Vector3us;	
	typedef Vector3<char>				Vector3c;	
	typedef Vector3<unsigned char>		Vector3uc;	


	template< typename T>
	inline  Vector3<T> operator -( const  Vector3<T>& vecA, const  Vector3<T>& vecB)
	{
		return  Vector3<T>( vecA.X - vecB.X, vecA.Y - vecB.Y, vecA.Z - vecB.Z);
	}
	
	template< typename T>
	inline  Vector3<T> operator +( const  Vector3<T>& vecA, const  Vector3<T>& vecB)
	{
		return  Vector3<T>( vecA.X + vecB.X, vecA.Y + vecB.Y, vecA.Z + vecB.Z);
	}
	
	template< typename T>
	inline  Vector3<T> operator *( const  Vector3<T>& vecA, const  Vector3<T>& vecB)
	{
		return  Vector3<T>( vecA.X * vecB.X, vecA.Y * vecB.Y, vecA.Z * vecB.Z);
	}
	
	template< typename T>
	inline  Vector3<T> operator *(const T value, const  Vector3<T>& vecA)
	{
		return  Vector3<T>(vecA.X * value, vecA.Y * value, vecA.Z * value);
	}
	
	template< typename T>
	inline  Vector3<T> operator /( const Vector3f& vecA, const  Vector3<T>& vecB)
	{
		return  Vector3<T>( vecA.X / vecB.X, vecA.Y / vecB.Y, vecA.Z / vecB.Z);
	}
	
	template< typename T>
	inline  Vector3<T> operator /(const T value, const  Vector3<T>& vecA)
	{
		return  Vector3<T>(vecA.X / value, vecA.Y / value, vecA.Z / value);
	}

	
	template< typename T>
	inline T Dot( const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
	};

	template< typename T>
	inline T GetLengthSquared( const Vector3<T>& a)
	{
		return Dot(a, a);
	};
	
	template< typename T>
	inline T GetLength( const Vector3<T>& a)
	{
		return sqrt( GetLengthSquared(a));
	};
	
	template< typename T>
	inline Vector3<T> Cross( const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.Y * b.Z - b.Y * a.Z,
						  a.Z * b.X - b.Z * a.X,
						  a.X * b.Y - b.X * a.Y);	
	};
	
	template< typename T>
	inline Vector3<T> Reflect( const Vector3<T>& inbound, const Vector3<T>& normal)
	{
		return inbound - 2.0f * Dot( inbound, normal) * normal;
	};
	
	template< typename T>
	inline T GetDistance(const Vector3<T>& a, const Vector3<T>& b)
	{
		const Vector3<T> diff = b - a;
		return GetLength(diff);
	};

	template< typename T>
	inline T GetDistanceSquared(const Vector3<T>& a, const Vector3<T>& b)
	{
		const Vector3<T> diff = b - a;
		return GetLengthSquared(diff);
	};
}