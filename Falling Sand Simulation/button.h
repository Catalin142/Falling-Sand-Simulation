#pragma once
#include <cstdint>

#include "utils.h"
#include "platform.h"

struct selection_button
{
	selection_button(uint32_t pos_x, uint32_t pos_y, uint32_t w, uint32_t h) : x(pos_x), y(pos_y), 
		width(w), height(h), texture(new uint32_t[size_t(width * height)]) { }

	~selection_button()
	{
		delete[] texture;
	}

	bool pressed(const vec2& mouse_pos)
	{
		return is_mouse_pressed(LEFT_BUTTON) && 
			mouse_pos.x > x && mouse_pos.x < x + width && mouse_pos.y > y && mouse_pos.y < y + height;
	}

	void render(graphics_context* ctx)
	{
		uint32_t pos_y = y;

		for (uint32_t i = 0; i < height; i++)
		{
			uint32_t pos_x = x;
			for (uint32_t j = 0; j < width; j++)
			{
				if (pos_x < ctx->width && pos_y < ctx->height)
					ctx->memory_buffer[pos_y * ctx->width + pos_x] = texture[i * width + j];
				pos_x += 1;
			}
			pos_y += 1;
		}
	}

	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint32_t* texture;
};
