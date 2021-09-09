#include "ScreenBuffer.h"
#include "../Utils/ColorConverter.h"

ScreenBuffer* ScreenBuffer::m_Instance;

ScreenBuffer::ScreenBuffer(std::shared_ptr<Window>& window, int width, int height) : m_Window(window)
{
	m_Width = width;
	m_Height = height;

	setViewport(m_Width, m_Height);

	m_BitMapInfo.bmiHeader.biSize = sizeof(m_BitMapInfo.bmiHeader);
	m_BitMapInfo.bmiHeader.biPlanes = 1;
	m_BitMapInfo.bmiHeader.biBitCount = sizeof(uint32_t) * 8;
	m_BitMapInfo.bmiHeader.biCompression = BI_RGB;

	m_Instance = this;
}

ScreenBuffer::~ScreenBuffer()
{
	VirtualFree(m_MemoryBuffer, 0, MEM_FREE);
}

void ScreenBuffer::setViewport(int width, int height)
{
	m_Width = width;
	m_Height = height;

	if (m_MemoryBuffer)
		VirtualFree(m_MemoryBuffer, 0, MEM_RELEASE);
	m_MemoryBuffer = VirtualAlloc(0, m_Width * m_Height * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	m_BitMapInfo.bmiHeader.biWidth = (LONG)m_Width;
	m_BitMapInfo.bmiHeader.biHeight = (LONG)m_Height;

	m_BufferSize = uint32_t(m_Width * m_Height);
}

void ScreenBuffer::Clear(float r, float g, float b)
{
	uint32_t* first = (uint32_t*)m_MemoryBuffer;
	uint32_t color = createHex(int(r * 255), int(g * 255), int(b * 255));

	for (unsigned int i = 0; i < m_BufferSize; i++)
		*(first++) = color;
}

void ScreenBuffer::Render()
{
	StretchDIBits(m_Window->getDeviceContext(), 0, 0, m_Window->getWidth(), m_Window->getHeight(), 1, 0, m_Width - 2, m_Height,
		m_MemoryBuffer, &m_BitMapInfo, DIB_RGB_COLORS, SRCCOPY);
}