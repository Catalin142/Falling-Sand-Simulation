#pragma once

#include <unordered_map>
#include <chrono>

#include "platform.h"
#include "cell.h"
#include "button.h"


struct sandbox
{
	sandbox(const wchar_t* name, uint32_t width, uint32_t height, uint32_t pixel_size = 1);
	~sandbox();

	void update();
	void put_circle(uint32_t pos_x, uint32_t pos_y, float radius);

	window* wnd;
	graphics_context* ctx;
	cell_world* cw;
	bool is_running = true;
	cell_type current_cell_type = cell_type::none;
	std::unordered_map<cell_type, selection_button*> buttons;
	float delta_time = 0.0f;

	std::chrono::time_point<std::chrono::system_clock> next_frame;
};