#include "Maths.h"

// quake algortihm
static float InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x * (1.5f - xhalf * x * x);
	return x;
}

vec2 operator+(const vec2& left, const vec2& right)
{
	return vec2(left.x + right.x, left.y + right.y);
}

vec2 operator-(const vec2& left, const vec2& right)
{
	return vec2(left.x - right.x, left.y - right.y);
}

vec2 operator*(const vec2& left, const vec2& right)
{
	return vec2(left.x * right.x, left.y * right.y);
}

vec2 operator/(const vec2& left, const vec2& right)
{
	return vec2(left.x / right.x, left.y / right.y);
}

vec2 operator*(const vec2& left, float scalar)
{
	return vec2(left.x * scalar, left.y * scalar);
}

vec2 operator/(const vec2& left, float scalar)
{
	return vec2(left.x / scalar, left.y / scalar);
}

vec3 operator+(const vec3& left, const vec3& right)
{
	return vec3(left.x + right.x, left.y + right.y, left.z + right.z);
}

vec3 operator-(const vec3& left, const vec3& right)
{
	return vec3(left.x - right.x, left.y - right.y, left.z - right.z);
}

vec3 operator*(const vec3& left, const vec3& right)
{
	return vec3(left.x * right.x, left.y * right.y, left.z * right.z);
}

vec3 operator/(const vec3& left, const vec3& right)
{
	return vec3(left.x / right.x, left.y / right.y, left.z / right.z);
}

vec3 operator*(const vec3& left, float scalar)
{
	return vec3(left.x * scalar, left.y * scalar, left.z * scalar);
}

vec3 operator/(const vec3& left, float scalar)
{
	return vec3(left.x / scalar, left.y / scalar, left.z / scalar);
}

float degToRad(float deg)
{
	return deg * (double)(PI / 180.0f);
}

float radToDeg(float rad)
{
	return rad * (double)(180 / PI);
}

vec2 vec2::normalize()
{
	float n = x * x + y * y;
	float invSqrt = InvSqrt(n);
	x *= invSqrt;
	y *= invSqrt;

	return *this;
}

float vec2::magnitude()
{
	float n = x * x + y * y;
	return InvSqrt(n) * n;
}

bool vec2::operator==(const vec2& other) const
{
	return (x == other.x && y == other.y);
}

vec3 vec3::normalize()
{
	float n = x * x + y * y + z * z;
	float invSqrt = InvSqrt(n);
	x *= invSqrt;
	y *= invSqrt;
	z *= invSqrt;

	return *this;
}

float vec3::magnitude()
{
	float n = x * x + y * y + z * z;
	return InvSqrt(n) * n;
}

bool vec3::operator==(const vec3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

vec2 lerp(const vec2& left, const vec2& right, float t)
{
	return vec2(lerp(left.x, right.x, t), lerp(left.y, right.y, t));
}

vec3 lerp(const vec3& left, const vec3& right, float t)
{
	return vec3(lerp(left.x, right.x, t), lerp(left.y, right.y, t), lerp(left.z, right.z, t));
}
