#pragma once

#include <windows.h>
#undef max
#undef min

#define LEFT_BUTTON VK_LBUTTON
#define KEY_R 'R'

struct vec2
{
	float x, y;
	vec2(float _x, float _y) : x(_x), y(_y) { }
};

static uint32_t max(uint32_t lhs, uint32_t rhs)
{
	return lhs > rhs ? lhs : rhs;
}

static uint32_t min(uint32_t lhs, uint32_t rhs)
{
	return lhs < rhs ? lhs : rhs;
}

static uint32_t get_random_int(uint32_t lb, uint32_t hb)
{
	return lb + rand() % (hb - lb + 1);
}

static float get_random_float()
{	
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

static bool is_mouse_pressed(uint32_t button)
{
	return GetKeyState(button) < 0;
}

static bool is_key_pressed(uint32_t key)
{
	return GetAsyncKeyState(key);
}