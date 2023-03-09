#include "platform.h"

uint32_t resize_event::width = 0;
uint32_t resize_event::height = 0;
bool resize_event::execute = false;

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		resize_event::width = LOWORD(lParam);
		resize_event::height = HIWORD(lParam);
		resize_event::execute = true;

	} break;

	case WM_CLOSE: DestroyWindow(hwnd);  break;
	case WM_DESTROY: PostQuitMessage(0); break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

window::window(const wchar_t* title, uint32_t w, uint32_t h) : width(w), height(h)
{
	DWORD flags = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
	resize_event::width = width;
	resize_event::height = height;

	window_class = { };

	window_instance = GetModuleHandle(NULL);
	window_class.lpfnWndProc = window_proc;
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.lpszClassName = L"WINDOW";

	RegisterClass(&window_class);

	handle = CreateWindowEx(0, L"WINDOW", title, flags, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, NULL, NULL, window_instance, NULL);

	if (!handle)
	{
		std::cout << "Eroare la fereastra";
		assert(false);
	}

	device_context = GetDC(handle);
	ShowWindow(handle, SW_SHOW);
}

void window::poll_events()
{
	MSG msg = { 0 };
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	if (msg.message == WM_QUIT)
		should_close = true;

	if (resize_event::execute)
	{
		width = resize_event::width;
		height = resize_event::height;

		resize_event::execute = false;
	}

	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

vec2 window::get_mouse_position()
{
	POINT p = { (LONG)0.0, (LONG)0.0 };
	GetCursorPos(&p);
	ScreenToClient(handle, &p);

	vec2 mouse_pos = { (float)p.x, (float)p.y };
	return mouse_pos;
}

graphics_context::graphics_context(window* w, uint32_t ps) : pixel_size(ps), wnd(w)
{
	width = wnd->width / pixel_size;
	height = wnd->height / pixel_size;
	buffer_size = width * height;

	memory_buffer = (uint32_t*)VirtualAlloc(0,
		width * height * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!memory_buffer)
	{
		std::cout << "Eroare la buffer!";
		assert(false);
	}

	bm_info.bmiHeader.biWidth = width;
	bm_info.bmiHeader.biHeight = height;

	bm_info.bmiHeader.biSize = sizeof(bm_info.bmiHeader);
	bm_info.bmiHeader.biPlanes = 1;
	bm_info.bmiHeader.biBitCount = sizeof(uint32_t) * 8;
	bm_info.bmiHeader.biCompression = BI_RGB;
}

// default white
void graphics_context::clear()
{
	memset(memory_buffer, 0xFFFFFF, buffer_size * sizeof(uint32_t));
}

void graphics_context::render()
{
	StretchDIBits(wnd->device_context,
		0, 0, wnd->width, wnd->height, 0, 0, width, height,
		memory_buffer, &bm_info, DIB_RGB_COLORS, SRCCOPY);
}

void graphics_context::convert_to_context(vec2& pos)
{
	vec2 norm = { (float)width / wnd->width, (float)height / wnd->height };
	pos.x = pos.x * norm.x;
	pos.y = pos.y * norm.y;

	pos.y = height - pos.y;
}