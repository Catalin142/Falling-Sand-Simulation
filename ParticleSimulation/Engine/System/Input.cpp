#include "Input.h"
#include "Window.h"
#include "ScreenBuffer.h"

vec2 getMousePosition()
{
	POINT p = { 0.0, 0.0 };
	GetCursorPos(&p);
	ScreenToClient(Window::Get()->getHandle(), &p);

	return vec2((float)p.x, (float)p.y);
}

vec2 WindowToBufferCoordonates(const vec2& vec)
{
	vec2 normalPos = vec;
	auto buffer = ScreenBuffer::Get();
	auto window = Window::Get();

	vec2 norm = { (float)buffer->getWidth() / (float)window->getWidth(), (float)buffer->getHeight() / (float)window->getHeight() };
	normalPos = normalPos * norm;

	normalPos.y = buffer->getHeight() - normalPos.y;

	return normalPos;
}