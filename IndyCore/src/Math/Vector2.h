// Juul Joosten 2013

#pragma once

#include <math.h>
#include <memory>

namespace Indy
{
	template< typename T>
	class Vector2
	{

	public:
		// ctors, copy ctors and assignment operators
		Vector2(void)
			:
		 X(0)
		,Y(0)
		{ }

		Vector2(T value)
			:
		 X(value)
		,Y(value)
		{ }

		Vector2(T x, T y)
			:
		 X(x)
		,Y(y)
		{ }

		Vector2(const Vector2<T>& cpyVec)
			:
		 X(cpyVec.X)
		,Y(cpyVec.Y)
		{ }

		Vector2(const T* xyz)
		{
			memcpy(XY, xy, sizeof(T) * 2);
		}


		Vector2<T>& operator =(const Vector2<T>& vecB)
		{
			memcpy(XY, vecB.XY, sizeof(T) * 2);
			return *this;
		}

		bool operator !=(const Vector2<T>& vecB)
		{
			return X != vecB.X || Y != vecB.Y;
		}


		Vector2<T> operator -()
		{  
			return Vector2<T>(-X, -Y);
		}

		Vector2<T> operator +(const Vector2<T>& vecB)
		{
			return Vector2<T>(X + vecB.X, Y + vecB.Y);
		}

		Vector2<T> operator -(const Vector2<T>& vecB)
		{
			return Vector2<T>(X - vecB.X, Y - vecB.Y);
		}

		Vector2<T> operator *(const Vector2<T>& vecB)
		{
			return Vector2<T>(X * vecB.X, Y * vecB.Y);
		}

		Vector2<T> operator *(const T value)
		{
			return Vector2<T>(X * value, Y * value);
		}

		Vector2<T> operator /(T value)
		{
			return Vector2<T>(X / value, Y / value);
		}

		Vector2<T>& operator +=(const Vector2<T>& vecB)
		{
			X += vecB.X;
			Y += vecB.Y;
			return *this;
		}

		Vector2<T>& operator -=(const Vector2<T>& vecB)
		{
			X -= vecB.X;
			Y -= vecB.Y;
			return *this;
		}

		Vector2<T>& operator *=(const Vector2<T>& vecB)
		{
			X *= vecB.X;
			Y *= vecB.Y;

			return *this;
		}

		Vector2<T>& operator *=(const T value)
		{
			X *= value;
			Y *= value;
			return *this;
		}

		Vector2<T>& operator /=(T value)
		{
			X /= value;
			Y /= value;
			return *this;
		}

		void Zero() 
		{ 
			X, Y = 0; 
		}

		Vector2<T>& Normalize()
		{
			T sqrtMag = GetLengthSquared(*this);

			if (sqrtMag > 0.0f)
			{
				T oneOverMag = 1.0f / sqrt(sqrtMag);
				X *= oneOverMag;
				Y *= oneOverMag;
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
			};
			struct 
			{
				T R;
				T G;
			};
			struct
			{
				T XY[2];
			};
			struct
			{
				T RG[2];
			};
		};

	};

	typedef Vector2<float>				Vector2f;
	typedef Vector2<double>				Vector2d;	
	typedef Vector2<unsigned int>		Vector2ui;
	typedef Vector2<int>				Vector2i;	
	typedef Vector2<short>				Vector2s;	
	typedef Vector2<unsigned short>		Vector2us;	
	typedef Vector2<char>				Vector2c;	
	typedef Vector2<unsigned char>		Vector2uc;	


	template< typename T>
	inline  Vector2<T> operator -(const  Vector2<T>& vecA, const  Vector2<T>& vecB)
	{
		return  Vector2<T>(vecA.X - vecB.X, vecA.Y - vecB.Y);
	}
	
	template< typename T>
	inline  Vector2<T> operator +(const  Vector2<T>& vecA, const  Vector2<T>& vecB)
	{
		return  Vector2<T>(vecA.X + vecB.X, vecA.Y + vecB.Y);
	}
	
	template< typename T>
	inline  Vector2<T> operator *(const  Vector2<T>& vecA, const  Vector2<T>& vecB)
	{
		return  Vector2<T>(vecA.X * vecB.X, vecA.Y * vecB.Y);
	}
	
	template< typename T>
	inline  Vector2<T> operator *(const T value, const  Vector2<T>& vecA)
	{
		return  Vector2<T>(vecA.X * value, vecA.Y * value);
	}
	
	template< typename T>
	inline  Vector2<T> operator /(const Vector2<T>& vecA, const  Vector2<T>& vecB)
	{
		return  Vector2<T>(vecA.X / vecB.X, vecA.Y / vecB.Y);
	}
	
	template< typename T>
	inline  Vector2<T> operator /(const T value, const  Vector2<T>& vecA)
	{
		return  Vector2<T>(vecA.X / value, vecA.Y / value);
	}

	
	template< typename T>
	inline T Dot(const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.X * b.X + a.Y * b.Y;
	};

	template< typename T>
	inline T GetLengthSquared(const Vector2<T>& a)
	{
		return Dot(a, a);
	};
	
	template< typename T>
	inline T GetLength(const Vector2<T>& a)
	{
		return sqrt( GetLengthSquared(a));
	};
	
	template< typename T>
	inline Vector2<T> Reflect(const Vector2<T>& inbound, const Vector2<T>& normal)
	{
		return inbound - 2.0f * Dot( inbound, normal) * normal;
	};
	
	template< typename T>
	inline T GetDistance(const Vector2<T>& a, const Vector2<T>& b)
	{
		const Vector2<T> diff = b - a;
		return GetLength(diff);
	};

	template< typename T>
	inline T GetDistanceSquared(const Vector2<T>& a, const Vector2<T>& b)
	{
		const Vector2<T> diff = b - a;
		return GetLengthSquared(diff);
	};
}