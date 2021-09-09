#include "../System/ScreenBuffer.h"

class Renderer
{
public:
	static void Init(std::shared_ptr<ScreenBuffer>& buffer);
	static void Clear(float r, float g, float b);

	static void setPixel(const vec2& pos, const vec3& color);
	static void setPixel(const vec2& pos, unsigned long hexColor);
	static bool checkPixel(const vec2& pos);

	static void Draw();

private:
	static std::shared_ptr<ScreenBuffer> m_Buffer;
	static uint32_t* m_BuffersBuffer; // ;-?
};