#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>

#include "utils.h"

// max 16!!!
enum class cell_type : uint8_t
{
	sand, 
	water, 
	cement,
	wood,
	smoke,
	fire,
	ember,

	none
};

enum particle_movement : uint16_t
{
	move_up				= 1 << 0, 
	move_down			= 1 << 1, 
	move_left			= 1 << 2, 
	move_right			= 1 << 3,
	move_down_left		= 1 << 4, 
	move_down_right		= 1 << 5,
	move_up_left		= 1 << 6, 
	move_up_right		= 1 << 7,
	move_none			= 1 << 8
};

enum direction
{
	down, up, left, right,
	down_right, down_left,
	up_right, up_left,
	count
};

#define DEFAULT_CELL 0b00000000101010111101100111111111
// false, type = none, color = background color #ABD9FF

class cell
{
	friend struct cell_world;
public:
	cell()
	{
		set_type(cell_type::none);
	}

	void set_color(uint32_t c);
	void set_type(const cell_type& type);
	void set_updated(bool u);
	void set_on_fire(bool u);

	uint32_t get_color();
	cell_type get_type();
	bool has_been_updated();
	bool is_on_fire();

	// 32 bits - first 24 bits = color					   ( 8 bits per channel )
	//		   - next 4 bits   = type			           ( max 2^4 = 16 types )
	//		   - 29th bit	   = updates this frame or not (true, false)
	//		   - 30th bit	   = if it's on fire
	uint32_t spec = DEFAULT_CELL;
					// false, type = none, color = background color #ABD9FF
	float timer = 1.0f;
};

struct particle_proprities
{
	std::vector<uint32_t> color_palette = { 0x00000000 };
	uint16_t movement = 0u;

	bool is_liquid = false;
	bool goes_through_liquid = false;

	bool is_flammable = false;

	float destroy_timer = 1.0f;
	float use_timer = false;

	vec2 random_velocity = { 0.0f, 0.0f };
};

struct cell_world
{
	cell_world(uint32_t width, uint32_t height);
	~cell_world()
	{
		delete[] cell_grid;
	}
	
	void update(float dt, const vec2& mouse_pos);

	cell* get_buffer();
	uint32_t get_buffer_size() { return grid_size; }

	cell& get_cell(uint32_t x, uint32_t y) { return cell_grid[y * width + x]; }

	void put_cell(cell_type type, uint32_t x, uint32_t y);
	void change_cell_type(cell& c, cell_type type);
	void swap_cells(uint32_t lhs_x, uint32_t lhs_y, uint32_t rhs_x, uint32_t rhs_y);

	bool check_coords(uint32_t x, uint32_t y)
	{
		if (x >= 1 && x < width - 1 && y >= 1 && y < height - 1)
			return true;
		return false;
	}

	void destroy_cell(cell& c)
	{
		c.spec = DEFAULT_CELL;
		c.set_type(cell_type::none);
	}

	void reset()
	{
		for (uint32_t i = 0; i < grid_size; i++)
			destroy_cell(cell_grid[i]);
	}

	cell* cell_grid = nullptr;

	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t grid_size = 0;

	std::unordered_map<cell_type, particle_proprities> proprieties;
};