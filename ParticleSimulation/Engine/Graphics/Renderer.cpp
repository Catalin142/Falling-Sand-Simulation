#include "Renderer.h"
#include "../Utils/ColorConverter.h"

std::shared_ptr<ScreenBuffer> Renderer::m_Buffer;
uint32_t* Renderer::m_BuffersBuffer; // ;-?

void Renderer::Init(std::shared_ptr<ScreenBuffer>& buffer)
{
	m_Buffer = buffer;
	m_BuffersBuffer = (uint32_t*)m_Buffer->m_MemoryBuffer;
}

void Renderer::Clear(float r, float g, float b)
{
	m_Buffer->Clear(r, g, b);
}

void Renderer::setPixel(const vec2& pos, const vec3& color)
{
	if (checkPixel(pos))
		return;

	int pixel = (int)pos.y * m_Buffer->m_Width + (int)pos.x;
	*(m_BuffersBuffer + pixel) = createHex(color.r * 255, color.g * 255, color.b * 255);
}

void Renderer::setPixel(const vec2& pos, unsigned long hexColor)
{
	if (checkPixel(pos))
		return;

	int pixel = (int)pos.y * m_Buffer->m_Width + (int)pos.x;
	*(m_BuffersBuffer + pixel) = hexColor;
}

bool Renderer::checkPixel(const vec2& pos)
{
	return (pos.x >= m_Buffer->m_Width || pos.y >= m_Buffer->m_Height || pos.x < 0 || pos.y <= 0);
}

void Renderer::Draw()
{
	m_Buffer->Render();
}
