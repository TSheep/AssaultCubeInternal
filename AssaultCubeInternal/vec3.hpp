#pragma once

#include <cstdint>
#include <math.h>

#define EPSILON 0.001f

class Vector3
{
public:

	float	x, y, z;

	Vector3();
	Vector3(float, float, float);

	static Vector3 up()
	{
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	bool	IsZero();
	
	float	&operator[](char);
	Vector3	&operator=(const Vector3 &);

	Vector3	&operator+=(const Vector3 &);
	Vector3	&operator-=(const Vector3 &);
	Vector3	&operator*=(const Vector3 &);
	Vector3	&operator*=(const float &);
	Vector3	&operator/=(const Vector3 &);
	Vector3	&operator/=(const float &);

	Vector3	operator+(const Vector3 &) const;
	Vector3	operator-(const Vector3 &) const;
	Vector3	operator*(const Vector3 &) const;
	Vector3	operator*(const float &) const;
	Vector3	operator/(const Vector3 &) const;
	Vector3	operator/(const float &) const;

	bool	operator==(const Vector3 &) const;
	bool	operator!=(const Vector3 &) const;

	float	DistTo(const Vector3 &) const;

	float	Length() const;
	float	LengthSquared() const;
	float	Length2D() const;
	float	Length2DSquared() const;

	Vector3	Cross(const Vector3 &) const;
	float	Dot(const Vector3 &) const;

	float	Normalize();

	void SwapYZ();
};
inline void Vector3::SwapYZ()
{
	float tmp = y;
	y = z;
	z = tmp;
}
inline Vector3::Vector3()
{
	x = y = z = 0.f;
}

inline bool Vector3::IsZero()
{
	return x == 0 && y == 0 && z == 0;
}
inline Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

inline float &Vector3::operator[](char i)
{
	return ((float *)this)[i];
}

inline Vector3 &Vector3::operator=(const Vector3 &v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

inline Vector3 &Vector3::operator+=(const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

inline Vector3 &Vector3::operator-=(const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

inline Vector3 &Vector3::operator*=(const Vector3 &v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

inline Vector3 &Vector3::operator*=(const float &f)
{
	x *= f;
	y *= f;
	z *= f;

	return *this;
}

inline Vector3 &Vector3::operator/=(const Vector3 &v)
{
	x /= v.x + EPSILON;
	y /= v.y + EPSILON;
	z /= v.z + EPSILON;

	return *this;
}

inline Vector3 &Vector3::operator/=(const float &f)
{
	x /= f + EPSILON;
	y /= f + EPSILON;
	z /= f + EPSILON;

	return *this;
}

inline Vector3 Vector3::operator+(const Vector3 &v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

inline Vector3 Vector3::operator-(const Vector3 &v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

inline Vector3 Vector3::operator*(const Vector3 &v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

inline Vector3 Vector3::operator*(const float &f) const
{
	return Vector3(x * f, y * f, z * f);
}

inline Vector3 Vector3::operator/(const Vector3 &v) const
{
	return Vector3(x / (v.x + EPSILON), y / (v.y + EPSILON), z / (v.z + EPSILON));
}

inline Vector3 Vector3::operator/(const float &f) const
{
	return Vector3(x / (f + EPSILON), y / (f + EPSILON), z / (f + EPSILON));
}

inline bool Vector3::operator==(const Vector3 &v) const
{
	return v.x == x && v.y == y && v.z == z;
}

inline bool Vector3::operator!=(const Vector3 &e) const
{
	return e.x != x || e.y != y || e.z != z;
}

inline float Vector3::DistTo(const Vector3 &v) const
{
	return (*this - v).Length();
}


inline float Vector3::Length() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

inline float Vector3::LengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

inline float Vector3::Length2D() const
{
	return sqrtf((x * x) + (y * y));
}

inline float Vector3::Length2DSquared() const
{
	return (x * x) + (y * y);
}

inline Vector3 Vector3::Cross(const Vector3 &e) const
{
	return Vector3((y * e.z) - (z * e.y), (z * e.x) - (x * e.z), (x * e.y) - (y * e.x));
}

inline float Vector3::Dot(const Vector3 &e) const
{
	return (x * e.x) + (y * e.y) + (z * e.z);
}

inline float Vector3::Normalize()
{
	float l = Length();
	float m = 1.f / (l + EPSILON);

	*this *= m;

	return l;
}

struct UnityRotator
{
	float                                              Yaw;                                                    
	float                                              Pitch;                                                 
	
	UnityRotator(float pitch, float yaw, float roll)
	{
		Pitch = pitch;
		Yaw = yaw;
	}
	UnityRotator()
	{
		Pitch = Yaw = 0;
	}
	UnityRotator operator + (UnityRotator other)
	{
		UnityRotator rot;
		rot.Pitch = Pitch + other.Pitch;
		rot.Yaw = Yaw + other.Yaw;

		return rot;
	}

	UnityRotator operator - (UnityRotator other)
	{
		UnityRotator rot;
		rot.Pitch = Pitch - other.Pitch;
		rot.Yaw = Yaw - other.Yaw;

		return rot;
	}

	UnityRotator clamp()
	{
		UnityRotator result;

		result.Pitch = Pitch;
		result.Yaw = Yaw;


		if (result.Pitch < -89)
			result.Pitch = -89;

		if (result.Pitch > 89)
			result.Pitch = 89;
		
		if (result.Yaw > 180)
			result.Yaw -= 360;
		else if (result.Yaw < -180)
			result.Yaw += 360;


		while (result.Yaw < -180.0f)
			result.Yaw += 360.0f;

		while (result.Yaw > 180.0f)
			result.Yaw -= 360.0f;


		return result;
	}

};

struct Quaternion
{
public:
	float x;

	float y;

	float z;

	float w;

	Vector3 operator *(Vector3 point)
	{
		Quaternion rotation = *this;

		float num = rotation.x * 2.0f;
		float num2 = rotation.y * 2.0f;
		float num3 = rotation.z * 2.0f;
		float num4 = rotation.x * num;
		float num5 = rotation.y * num2;
		float num6 = rotation.z * num3;
		float num7 = rotation.x * num2;
		float num8 = rotation.x * num3;
		float num9 = rotation.y * num3;
		float num10 = rotation.w * num;
		float num11 = rotation.w * num2;
		float num12 = rotation.w * num3;
		Vector3 result;
		result.x = (1.0f - (num5 + num6)) * point.x + (num7 - num12) * point.y + (num8 + num11) * point.z;
		result.y = (num7 + num12) * point.x + (1.0f - (num4 + num6)) * point.y + (num9 - num10) * point.z;
		result.z = (num8 - num11) * point.x + (num9 + num10) * point.y + (1.0f - (num4 + num5)) * point.z;
		return result;
	}

};