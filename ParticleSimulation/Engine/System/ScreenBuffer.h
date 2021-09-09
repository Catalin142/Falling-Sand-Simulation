#pragma once
#include <memory>
#include "Window.h"

class ScreenBuffer
{
	friend class Renderer;

public:
	ScreenBuffer(std::shared_ptr<Window>& window, int width, int height);
	~ScreenBuffer();

	void setViewport(int width, int height);
	void Clear(float r, float g, float b);

	void* getBuffer() { return m_MemoryBuffer; }
	int getWidth() { return m_Width; }
	int getHeight() { return m_Height; }
	float getAspectRatio() { return (float)m_Width / (float)m_Height; }

	void Render();

	static ScreenBuffer* Get() { return m_Instance; }

private:
	void* m_MemoryBuffer = nullptr;
	BITMAPINFO m_BitMapInfo;

	std::shared_ptr<Window> m_Window;
	uint32_t m_BufferSize;

	int m_Width;
	int m_Height;

	static ScreenBuffer* m_Instance;
};