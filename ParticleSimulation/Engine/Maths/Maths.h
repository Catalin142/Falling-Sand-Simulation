#pragma once

#define PI 3.14159265358979323846f
class vec2
{
	friend vec2 operator+(const vec2& left, const vec2& right);
	friend vec2 operator-(const vec2& left, const vec2& right);
	friend vec2 operator*(const vec2& left, const vec2& right);
	friend vec2 operator/(const vec2& left, const vec2& right);

	friend vec2 operator*(const vec2& left, float scalar);
	friend vec2 operator/(const vec2& left, float scalar);


public:
	vec2() = default;
	vec2(float x_, float y_) : x(x_), y(y_) { }

	float x;
	float y;

	vec2 normalize();
	float magnitude();

	bool operator==(const vec2& other) const;
};

class vec3
{
	friend vec3 operator+(const vec3& left, const vec3& right);
	friend vec3 operator-(const vec3& left, const vec3& right);
	friend vec3 operator*(const vec3& left, const vec3& right);
	friend vec3 operator/(const vec3& left, const vec3& right);

	friend vec3 operator*(const vec3& left, float scalar);
	friend vec3 operator/(const vec3& left, float scalar);

public:
	vec3() = default;
	vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) { }

	union { float x, r; };
	union { float y, g; };
	union { float z, b; };

	vec3 normalize();
	float magnitude();

	bool operator==(const vec3& other) const;
};

static float Clamp(float value, float min, float max)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	return value;
}

float degToRad(float deg);
float radToDeg(float rad);

float lerp(float a, float b, float t);
vec2 lerp(const vec2& left, const vec2& right, float t);
vec3 lerp(const vec3& left, const vec3& right, float t);
