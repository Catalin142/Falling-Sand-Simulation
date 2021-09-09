#include "Engine/System/Window.h"
#include "Engine/System/ScreenBuffer.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/System/Input.h"

#include "Simulation/ParticleEmmiter.h"

#include <memory>
#include <chrono>
#include <iostream>

int main()
{
	std::shared_ptr<Window> m_Window = std::make_shared<Window>(L"Particle Simulation", 900, 900);
	std::shared_ptr<ScreenBuffer> m_Buffer = std::make_shared<ScreenBuffer>(m_Window, 300, 300);

	Renderer::Init(m_Buffer);
	ParticleEmmiter::Init(m_Buffer);
	
	std::chrono::time_point<std::chrono::system_clock> timeNow = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> oldTime = timeNow;

	float deltaTime = 0.0f;

	bool SpaceState = false;
	ParticleType CurrentType = ParticleType::Sand;

	while (m_Window->isRunning())
	{
		timeNow = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<float>(timeNow - oldTime).count();

		if (!m_Window->isMoved())
		{
			Renderer::Clear(0.0f, 0.0f, 0.0f);

			bool state = GetAsyncKeyState(VK_SPACE);

			for(int x = 0; x < 7; x++)
				for (int y = m_Buffer->getHeight() - 6; y < m_Buffer->getHeight(); y++)
				{
					switch (CurrentType)
					{
					case ParticleType::Sand:
						Renderer::setPixel({ (float)x, (float)y }, 0xffff00);  
						break;

					case ParticleType::Water:
						Renderer::setPixel({ (float)x, (float)y }, 0x0000ff); 
						break;

					case ParticleType::Wood:
						Renderer::setPixel({ (float)x, (float)y }, 0xba8c63);  
						break;

					case ParticleType::None:
						Renderer::setPixel({ (float)x, (float)y }, 0xffffff);
						break;
					}
				}

			if (state != SpaceState && state == true)
			{
				int type = (int)CurrentType + 1;
				if (type > (int)ParticleType::Wood)
					type = (int)ParticleType::None;
				CurrentType = (ParticleType)type;

				SpaceState = true;
			}
			if (state == false)
				SpaceState = false;

			if (GetAsyncKeyState(VK_MOUSE_LEFT))
			{
				auto pos = WindowToBufferCoordonates(getMousePosition());
				if (pos.x >= 0 && pos.x < m_Buffer->getWidth() && pos.y >= 0 && pos.y < m_Buffer->getHeight() - 1)
				{
					ParticleEmmiter::setCell(CurrentType, (int)pos.x,     (int)pos.y);
					ParticleEmmiter::setCell(CurrentType, (int)pos.x - 1, (int)pos.y);
					ParticleEmmiter::setCell(CurrentType, (int)pos.x,     (int)pos.y + 1);
					ParticleEmmiter::setCell(CurrentType, (int)pos.x - 1, (int)pos.y + 1);
				}
			}

			if (GetAsyncKeyState('W'))
				deltaTime = 0.0f;


			ParticleEmmiter::Update(deltaTime);

			Renderer::Draw();
		}
		
		m_Window->pollEvents();
		oldTime = timeNow;
	}
}