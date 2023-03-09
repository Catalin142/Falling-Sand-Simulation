#pragma once

#include <windows.h>
#include <cstdint>
#include <iostream>
#include <assert.h>

#include "utils.h"

struct window
{
	window() = default;
	window(const wchar_t* title, uint32_t width, uint32_t height);

	void poll_events();
	vec2 get_mouse_position();

	HWND handle;
	WNDCLASS window_class;
	HINSTANCE window_instance;
	HDC device_context;

	uint32_t width;
	uint32_t height;
	bool should_close = false;
};

struct graphics_context
{
	graphics_context() = default;
	graphics_context(window* w, uint32_t pixel_size);
	~graphics_context()
	{
		VirtualFree(memory_buffer, 0, MEM_FREE);
	}

	void clear();
	void render();

	void convert_to_context(vec2& pos);

	uint32_t* memory_buffer;
	window* wnd;
	BITMAPINFO bm_info;

	uint32_t buffer_size;
	uint32_t width;
	uint32_t height;
	uint32_t pixel_size;
};

struct resize_event
{
	static uint32_t width;
	static uint32_t height;
	static bool execute;
};
