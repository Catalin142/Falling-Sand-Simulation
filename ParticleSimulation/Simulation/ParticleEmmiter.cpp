#include "ParticleEmmiter.h"
#include "../Engine/Graphics/Renderer.h"
#include "../Engine/Utils/Random.h"
#include <iostream>

ParticleType* ParticleEmmiter::m_ParticleOutput;

int ParticleEmmiter::m_PoolSize;
int ParticleEmmiter::m_Width;
int ParticleEmmiter::m_Height;

void ParticleEmmiter::Init(const std::shared_ptr<ScreenBuffer>& buffer)
{
	m_Width = buffer->getWidth();
	m_Height = buffer->getHeight();
	m_PoolSize = m_Width * m_Height;

	m_ParticleOutput = new ParticleType[m_PoolSize];

	for (int i = 0; i < m_PoolSize; i++)
		m_ParticleOutput[i] = ParticleType::None;
}

void ParticleEmmiter::setCell(ParticleType type, int x, int y)
{
	m_ParticleOutput[y * m_Width + x] = type;
}

void ParticleEmmiter::Update(float deltaTime)
{
	static float thefinalCountdown = 0.0f;

	bool canUpdate = false;

	if (thefinalCountdown > 0.01f)
	{
		canUpdate = true;
		thefinalCountdown = 0.0f;
	}

	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			if (m_ParticleOutput[y * m_Width + x] != ParticleType::None)
			{
				switch (m_ParticleOutput[y * m_Width + x])
				{
				case ParticleType::Sand:
					Renderer::setPixel({ (float)x, (float)y }, 0xffff00);
					if (canUpdate)
						SandUpdate(x, y);
					break;

				case ParticleType::Water:
					Renderer::setPixel({ (float)x, (float)y }, 0x0000ff);
						if (canUpdate)
					WaterUpdate(x, y);
					break;

				case ParticleType::Wood:
					Renderer::setPixel({ (float)x, (float)y }, 0xba8c63);
					break;
				}
			}
		}

	thefinalCountdown += deltaTime;
}

ParticleType ParticleEmmiter::getType(int x, int y)
{
	return m_ParticleOutput[y * m_Width + x];
}

void ParticleEmmiter::moveCell(int srcx, int srcy, int destx, int desty)
{
	if (m_ParticleOutput[desty * m_Width + destx] == ParticleType::None)
	{
		auto part = m_ParticleOutput[srcy * m_Width + srcx];
		m_ParticleOutput[srcy * m_Width + srcx] = ParticleType::None;
		m_ParticleOutput[desty* m_Width + destx] = part;
	}
}

ParticleType& ParticleEmmiter::getCell(int x, int y)
{
	return m_ParticleOutput[y * m_Width + x];
}

void ParticleEmmiter::SandUpdate(int x, int y)
{
	if (x > 0 && x < m_Width - 1 && y > 0 && y < m_Height)
	{
		if (getType(x, y - 1) == ParticleType::None)
			moveCell(x, y, x, y - 1);

		else if (getType(x + 1, y - 1) == ParticleType::None)
			moveCell(x, y, x + 1, y - 1);

		else if (getType(x - 1, y - 1) == ParticleType::None)
			moveCell(x, y, x - 1, y - 1);
	}
}

void ParticleEmmiter::WaterUpdate(int x, int y)
{
	if (x > 0 && x < m_Width - 1 && y > 0 && y < m_Height)
	{
		if (getType(x, y - 1) == ParticleType::None)
			moveCell(x, y, x, y - 1);

		else if (getType(x + 1, y - 1) == ParticleType::None)
			moveCell(x, y, x + 1, y - 1);

		else if (getType(x - 1, y - 1) == ParticleType::None)
			moveCell(x, y, x - 1, y - 1);

		bool left = false, right = false;
		if (getType(x - 1, y) == ParticleType::None)
			left = true;

		if (getType(x + 1, y) == ParticleType::None)
			right = true;

		if (left && right)
		{
			if (Random::Float() - 0.6f >= 0)
				right = false;
			else left = false;
		}

		if (right)
			moveCell(x, y, x + 1, y);
		else moveCell(x, y, x - 1, y);
	}
}