#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

#include "../Maths/Maths.h"
class Window
{
	friend LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Window(const wchar_t* name, uint32_t width, uint32_t height, unsigned long flags = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX);

	void pollEvents();

	uint32_t getWidth() { return m_Width; }
	uint32_t getHeight() { return m_Height; }

	vec2 getClientRectSize();
	
	HWND& getHandle() { return m_WindowHandle; }
	HDC& getDeviceContext() { return m_DeviceContext; }

	void setWidth(uint32_t width) { m_Width = width; }
	void setHeight(uint32_t height) { m_Height = height; }

	bool& isRunning() { return m_isRunning; }
	bool isMoved() { return m_isMoved; }
	std::wstring getName() { return std::wstring(m_Name); }

	static Window* Get() { return m_Instance; }

private:
	HWND m_WindowHandle;
	WNDCLASS m_WindowClass;
	HDC m_DeviceContext;

	uint32_t m_Width;
	uint32_t m_Height;

	wchar_t* m_Name;
	bool m_isRunning;

	bool m_isMoved = false;

	static Window* m_Instance;
};