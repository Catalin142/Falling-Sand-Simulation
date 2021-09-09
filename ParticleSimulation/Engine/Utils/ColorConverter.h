#pragma once

#include "../Maths/Maths.h"

static unsigned long createHex(int r, int g, int b)
{
	//return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
	return ((r << 16) | (g << 8) | b);
}

static unsigned long createHex(const vec3& color)
{
	//return ((int(color.x * 255) & 0xff) << 16) + ((int(color.y * 255) & 0xff) << 8) + (int(color.z * 255) & 0xff);
	return (int(color.r * 255) << 16 | int(color.g * 255) << 8 | int(color.b * 255));
}

static vec3 createRGB(unsigned long hex)
{
	return vec3(float((hex >> 16) & 0xff), float((hex >> 8) & 0xff), float((hex) & 0xff)) / 255.0f;
}