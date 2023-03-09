#include <chrono>
#include <string>
#include <thread>
#include "sandbox.h"

sandbox::sandbox(const wchar_t* name, uint32_t width, uint32_t height, uint32_t pixel_size)
{
	wnd = new window(name, width, height);
	ctx = new graphics_context(wnd, pixel_size);
	cw = new cell_world(ctx->width, ctx->height - 20);

	uint32_t button_x = 10;
	uint32_t button_y = 185;
	uint32_t button_width = 10;
	uint32_t button_height = 10;
	uint32_t spacing = 5;

	for (auto& type : cw->proprieties)
	{
		cell_type c_type = type.first;
		buttons[c_type] = new selection_button(button_x, button_y, button_width, button_height);
		button_x += (button_width + spacing);

		for (uint32_t index = 0; index < button_width * button_height; index++)
		{
			uint32_t pallete_size = (uint32_t)cw->proprieties[c_type].color_palette.size();
			*(buttons[c_type]->texture + index) = cw->proprieties[c_type].color_palette[get_random_int(0, pallete_size - 1)];
		}
	}

	next_frame = std::chrono::system_clock::now();
}

sandbox::~sandbox()
{
	for (auto& button : buttons)
		delete[] button.second->texture;

	delete cw;
	delete ctx;
	delete wnd;
}

std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
std::chrono::time_point<std::chrono::system_clock> old = now;

void sandbox::put_circle(uint32_t pos_x, uint32_t pos_y, float radius)
{
	int32_t x = -(int32_t)radius;
	int32_t y = 0;
	float err = 2 - 2 * radius;

	while (x < 0)
	{
		for (uint32_t i = pos_x + x; i < pos_x - x; i++)
		{
			cw->put_cell(current_cell_type, i, pos_y + y);
			cw->put_cell(current_cell_type, i, pos_y - y);
		}

		cw->put_cell(current_cell_type, pos_x - x, pos_y + y);
		cw->put_cell(current_cell_type, pos_x - y, pos_y - x);
		cw->put_cell(current_cell_type, pos_x + x, pos_y - y);
		cw->put_cell(current_cell_type, pos_x + y, pos_y + x);

		radius = (float)err;

		if (radius <= y) err += ++y * 2 + 1;
		if (radius > x || err > y) err += ++x * 2 + 1;
	}
}

void sandbox::update()
{
	now = std::chrono::system_clock::now();
	delta_time = std::chrono::duration<float>(now - old).count();
	old = now;

	// ~144 fps
	next_frame += std::chrono::milliseconds(1000 / 144);

	vec2 mouse_pos = wnd->get_mouse_position();
	ctx->convert_to_context(mouse_pos);

	for (auto& button : buttons)
	{
		button.second->render(ctx);
		if (button.second->pressed(mouse_pos))
			current_cell_type = button.first;
	}

	if (is_mouse_pressed(LEFT_BUTTON))
		put_circle((uint32_t)mouse_pos.x, (uint32_t)mouse_pos.y, 3.0f);

	if (is_key_pressed(KEY_R))
		cw->reset();

	cw->update(delta_time, mouse_pos);

	memcpy(ctx->memory_buffer, cw->cell_grid, cw->get_buffer_size() * sizeof(uint32_t));

	for (uint32_t i = 0; i < cw->get_buffer_size(); i++)
		cw->cell_grid[i].spec &= 0xEFFFFFFF;

	ctx->render();
	wnd->poll_events();
	is_running = !wnd->should_close;

	std::this_thread::sleep_until(next_frame);
}