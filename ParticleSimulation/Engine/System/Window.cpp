#include <iostream>
#include <assert.h>

#include "Window.h"

Window* Window::m_Instance = nullptr;

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		auto window = Window::Get();
		window->m_Width = LOWORD(lParam);
		window->m_Height = HIWORD(lParam);

	} break;

	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	} break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} break;

	case WM_MOVE:
	{
		Window::Get()->m_isMoved = true;
	} break;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::Window(const wchar_t* name, uint32_t width, uint32_t height, unsigned long flags) : m_Name(const_cast<wchar_t*>(name)), m_Width(width), m_Height(height), m_isRunning(true)
{
	//ShowCursor(false);
	m_Instance = this;

	m_WindowClass = { };

	HINSTANCE hInstance = GetModuleHandle(NULL);

	m_WindowClass.lpfnWndProc = WindowProcedure;
	m_WindowClass.hInstance = GetModuleHandle(NULL);
	m_WindowClass.lpszClassName = L"Window Class";

	RegisterClass(&m_WindowClass);

	m_WindowHandle = CreateWindowEx(0, L"Window Class", m_Name, flags,
		CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height,
		NULL, NULL, hInstance, NULL);

	if (!m_WindowHandle)
	{
		std::cout << "Nu s-a putut creea fereastra" << std::endl;
		assert(m_WindowHandle);
	}

	m_DeviceContext = GetDC(m_WindowHandle);

	RECT r;
	GetWindowRect(m_WindowHandle, &r);
	ClipCursor(&r);

	ShowWindow(m_WindowHandle, SW_SHOW);
}

void Window::pollEvents()
{
	m_isMoved = false;
	MSG msg = { };
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	if (msg.message == WM_QUIT)
	{
		m_isRunning = false;
	}

	TranslateMessage(&msg);
	DispatchMessage(&msg);
}

vec2 Window::getClientRectSize()
{
	RECT rct;
	GetClientRect(Window::Get()->getHandle(), &rct);
	return { (float)rct.right, (float)rct.bottom };
}
