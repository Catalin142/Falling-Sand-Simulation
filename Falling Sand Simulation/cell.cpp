#include "cell.h"

#include <iostream>

void cell::set_color(uint32_t c)
{
	c &= 0x00ffffff;
	spec &= 0xff000000;
	spec |= c;
}

void cell::set_type(const cell_type& type)
{
	uint32_t type_id = (uint32_t)type;
	type_id &= 0x0000000f;
	type_id <<= 24;
	spec &= 0xf0ffffff;
	spec |= type_id;
}

void cell::set_updated(bool u)
{
	uint32_t update = u << 28;
	spec &= 0xEfffffff;
	spec |= update;
}

void cell::set_on_fire(bool u)
{
	uint32_t update = u << 29;
	spec &= 0xDfffffff;
	spec |= update;
}

uint32_t cell::get_color()
{
	uint32_t c = spec & 0x00ffffff;
	return c;
}

cell_type cell::get_type()
{
	uint32_t type = spec & 0x0f000000;
	type = type >> 24;
	return cell_type(type);
}

bool cell::has_been_updated()
{
	uint32_t up = (spec & 0x10000000) >> 28;
	return bool(up);
}

bool cell::is_on_fire()
{
	uint32_t up = (spec & 0x20000000) >> 29;
	return bool(up);
}

cell_world::cell_world(uint32_t width, uint32_t height) : grid_size(width* height),
width(width), height(height)
{
	cell_grid = new cell[grid_size];

	particle_proprities none_props;
	none_props.color_palette = { 0xABD9FF };

	particle_proprities sand_props;
	sand_props.color_palette = { 0xD1C59F, 0xC2B280, 0xB39F61, 0x9C884B, 0x7D6D3C };
	sand_props.movement = particle_movement::move_down | particle_movement::move_down_left | particle_movement::move_down_right;
	sand_props.goes_through_liquid = true;

	particle_proprities water_props;
	water_props.color_palette = { 0x2389DA };
	//water_props.color_palette = { 0x0F5E9C, 0x2389DA, 0x1CA3EC, 0x74CCF4, 0xABCD8 };
	water_props.movement = particle_movement::move_down | particle_movement::move_down_left | particle_movement::move_down_right
		| particle_movement::move_left | particle_movement::move_right;
	water_props.is_liquid = true;
	water_props.dispersion_rate = 5;

	particle_proprities cement_props;
	cement_props.color_palette = { 0xB2BEB5, 0x676767, 0x555555 };
	cement_props.movement = particle_movement::move_none;

	particle_proprities wood_props;
	wood_props.color_palette = { 0x563232, 0xDEB887 };
	wood_props.movement = particle_movement::move_none;
	wood_props.is_flammable = true;
	wood_props.destroy_timer = 1.0f;

	particle_proprities smoke_props;
	smoke_props.color_palette = { 0x474747 };
	smoke_props.movement = particle_movement::move_up | particle_movement::move_up_left | particle_movement::move_up_right
		| particle_movement::move_left | particle_movement::move_right;
	smoke_props.is_liquid = true;
	smoke_props.destroy_timer = 0.5f;
	smoke_props.use_timer = true;

	particle_proprities fire_props;
	fire_props.color_palette = { 0xFFE808, 0xFF9A00 };
	fire_props.movement = particle_movement::move_up | particle_movement::move_up_left | particle_movement::move_up_right
		| particle_movement::move_left | particle_movement::move_right | particle_movement::move_down
		| particle_movement::move_down_left | particle_movement::move_down_right;
	fire_props.is_flammable = true;
	fire_props.destroy_timer = 0.2f;
	fire_props.use_timer = true;
	fire_props.random_velocity = {5, 5};

	particle_proprities ember_props;
	ember_props.color_palette = { 0x9A2D19, 0xC73915, 0xE19C18 };
	ember_props.movement = particle_movement::move_none;
	ember_props.is_flammable = true;

	proprieties[cell_type::none]	= none_props;
	proprieties[cell_type::sand]	= sand_props;
	proprieties[cell_type::water]	= water_props;
	proprieties[cell_type::cement]	= cement_props;
	proprieties[cell_type::wood]	= wood_props;
	proprieties[cell_type::smoke]	= smoke_props;
	proprieties[cell_type::fire]	= fire_props;
	proprieties[cell_type::ember]	= ember_props;
}

void cell_world::update(float dt, const vec2& mouse_pos)
{
	for (uint32_t y = 1; y < height - 1; y++)
		for (uint32_t x = 1; x < width - 1; x++)
		{
			cell& c = get_cell(x, y);
			bool is_in_water = false;
			if (get_surrounding_cell(x, y, direction::up).get_type() == cell_type::water)
				is_in_water = true;

			if (c.has_been_updated() || c.get_type() == cell_type::none)
				continue;

			if (proprieties[c.get_type()].use_timer)
				c.timer -= dt;

			else if (proprieties[c.get_type()].is_flammable && c.is_on_fire())
			{
				if (is_in_water)
				{
					c.set_on_fire(false);
					continue;
				}

				c.timer -= dt;
				if (c.timer > 0.8f)
					put_cell(cell_type::smoke, x, y + 1);
				if (c.get_type() != cell_type::ember)
					change_cell_type(c, cell_type::ember);
			}

			if (c.timer <= 0.0f)
			{
				if (c.is_on_fire())
				{
					for (uint32_t i = 0; i < direction::count; i++)
					{
						cell& sur_cell = get_surrounding_cell(x, y, (direction)i);
						if (proprieties[sur_cell.get_type()].is_flammable)
						{
							sur_cell.set_on_fire(true);
							sur_cell.timer = get_random_float() / 2.0f;
						}
					}
					put_cell(cell_type::smoke, x, y + 1);
				}

				destroy_cell(c);
				continue;
			}

			uint16_t movement = proprieties[c.get_type()].movement;
			if (movement & particle_movement::move_none) // is particle doesn t move
				continue;
			
			uint32_t p_x = x;
			uint32_t p_y = y;

			// lambda to stop repeating code
			auto move_particle = [&](particle_movement mvm_type, direction dir, uint32_t next_x, uint32_t next_y) -> bool
			{
				bool can_move = movement & mvm_type;
				if (!can_move)
					return false;

				if (!check_coords(next_x, next_y))
					return false;

				cell& sur_cell = get_surrounding_cell(p_x, p_y, dir);
				bool dest_cell_empty = sur_cell.get_type() == cell_type::none;

				bool src_cell_goes_thought_liquid = proprieties[c.get_type()].goes_through_liquid;
				bool dest_cell_is_liquid = proprieties[sur_cell.get_type()].is_liquid;

				bool can_swap = src_cell_goes_thought_liquid && dest_cell_is_liquid;
				if (!dest_cell_empty && !can_swap)
					return false;

				bool dest_cell_updated = sur_cell.has_been_updated();
				if (dest_cell_updated)
					return false;

				c.set_updated(true);
				swap_cells(p_x, p_y, next_x, next_y);
				p_x = next_x;
				p_y = next_y;
				return true;
			};

			int direction = get_random_int(0, 1);

			// down
			move_particle(particle_movement::move_down, direction::down, p_x, p_y - 1);

			// up
			move_particle(particle_movement::move_up, direction::up, p_x, p_y + 1);

			// down left - down right
			if (direction == 0) move_particle(particle_movement::move_down_right, direction::down_right, p_x + 1, p_y - 1);
			else				move_particle(particle_movement::move_down_left, direction::down_left, p_x - 1, p_y - 1);

			// up left - up right
			if (direction == 0) move_particle(particle_movement::move_up_right, direction::up_right, p_x + 1, p_y + 1);
			else				move_particle(particle_movement::move_up_left, direction::up_left, p_x - 1, p_y + 1);

			// left - right
			if (direction == 0)
			{
				uint32_t dest_x = get_first_available_cell(p_x, p_y, 1, proprieties[c.get_type()].dispersion_rate);
				move_particle(particle_movement::move_right, direction::right, dest_x, p_y);
			}
			else
			{
				uint32_t dest_x = get_first_available_cell(p_x, p_y, -1, proprieties[c.get_type()].dispersion_rate);
				move_particle(particle_movement::move_left, direction::left, dest_x, p_y);
			}
		}

}

void cell_world::put_cell(cell_type type, uint32_t x, uint32_t y)
{
	if (!check_coords(x, y))
		return;

	cell& c = get_cell(x, y);
	if (c.get_type() != cell_type::none && type != cell_type::none)
		return;

	c.set_type(type);
	c.set_color(proprieties[type].color_palette[get_random_int(0, (uint32_t)proprieties[type].color_palette.size() - 1)]);
	c.set_updated(false);

	if (type == cell_type::fire || type == cell_type::ember)
		c.set_on_fire(true);
	else
		c.set_on_fire(false);

	c.timer = proprieties[type].destroy_timer;
}

void cell_world::change_cell_type(cell& c, cell_type type)
{
	c.set_type(type);
	c.set_color(proprieties[type].color_palette[get_random_int(0, (uint32_t)proprieties[type].color_palette.size() - 1)]);
}

void cell_world::swap_cells(uint32_t lhs_x, uint32_t lhs_y, uint32_t rhs_x, uint32_t rhs_y)
{
	uint32_t& left_spec = get_cell(lhs_x, lhs_y).spec;
	uint32_t& right_spec = get_cell(rhs_x, rhs_y).spec;
	uint32_t lhs = left_spec;
	left_spec = right_spec;
	right_spec = lhs;

	float& left_timer = get_cell(lhs_x, lhs_y).timer;
	float& right_timer = get_cell(rhs_x, rhs_y).timer;
	float lhs_t = left_timer;
	left_timer = right_timer;
	right_timer = lhs_t;
}
