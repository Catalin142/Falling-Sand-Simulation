#include "ParticleEmmiter.h"
#include "../Engine/Graphics/Renderer.h"
#include "../Engine/Utils/Random.h"
#include <iostream>

ParticleEmmiter::Particle* ParticleEmmiter::m_ParticleOutput;

std::vector<ParticleEmmiter::ParticleMovement> ParticleEmmiter::m_FutureMovement;

int ParticleEmmiter::m_PoolSize;
int ParticleEmmiter::m_Width;
int ParticleEmmiter::m_Height;

void ParticleEmmiter::Init(const std::shared_ptr<ScreenBuffer>& buffer)
{
	m_Width = buffer->getWidth();
	m_Height = buffer->getHeight();
	m_PoolSize = m_Width * m_Height;

	m_ParticleOutput = new Particle[m_PoolSize]{};
	m_FutureMovement.reserve(m_PoolSize);
}

void ParticleEmmiter::setCell(ParticleType type, int x, int y)
{
	m_ParticleOutput[y * m_Width + x].Type = type;
}

void ParticleEmmiter::Update(float deltaTime)
{
	auto remove = std::remove_if(m_FutureMovement.begin(), m_FutureMovement.end(), [&](const auto& elem) -> bool
		{
			return (m_ParticleOutput[elem.destIndex].Type != ParticleType::None);
		});

	m_FutureMovement.erase(remove, m_FutureMovement.end());

	std::sort(m_FutureMovement.begin(), m_FutureMovement.end(), [](const auto& left, const auto& right)
		{
			return left.destIndex > right.destIndex;
		});

	for (const auto& mvm : m_FutureMovement)
	{
		if (m_ParticleOutput[mvm.destIndex].Type == ParticleType::None)
		{
			auto part = m_ParticleOutput[mvm.srcIndex];
			m_ParticleOutput[mvm.srcIndex].Type = ParticleType::None;
			m_ParticleOutput[mvm.destIndex] = part;
		}
	}

	m_FutureMovement.clear();

	static float thefinalCountdown = 0.0f;

	bool canUpdate = false;

	if (thefinalCountdown > 0.01f)
	{
		canUpdate = true;
		thefinalCountdown = 0.0f;
	}
	for (int i = 0; i < m_PoolSize; i++)
		m_ParticleOutput[i].Updated = false;

	for (int y = 1; y < m_Height - 1; y++)
		for (int x = 1; x < m_Width - 1; ++x)
		{
			if (m_ParticleOutput[y * m_Width + x].Type != ParticleType::None && m_ParticleOutput[y * m_Width + x].Updated == false)
			{
				switch (m_ParticleOutput[y * m_Width + x].Type)
				{
				case ParticleType::Sand:
					Renderer::setPixel({ (float)x, (float)y }, 0xe1bf92);
					if (canUpdate)
						SandUpdate(x, y);
					break;

				case ParticleType::Water:
					Renderer::setPixel({ (float)x, (float)y }, 0x2389da);
					if (canUpdate)
						WaterUpdate(x, y);
					break;

				case ParticleType::Stone:
					Renderer::setPixel({ (float)x, (float)y }, 0x5a4d41);
					break;

				case ParticleType::Smoke:
					Renderer::setPixel({ (float)x, (float)y }, 0x848884);
					if (canUpdate)
						SmokeUpdate(x, y);
					break;

				case ParticleType::Lava:
					Renderer::setPixel({ (float)x, (float)y }, 0xcf1020);
					if (canUpdate)
						LavaUpdate(x, y);
					break;
				
				case ParticleType::Rock:
					Renderer::setPixel({ (float)x, (float)y }, 0x808487);
					if (canUpdate)
						SandUpdate(x, y);
					break;
				}
			}
		}

	thefinalCountdown += deltaTime;
}

ParticleType ParticleEmmiter::getType(int x, int y)
{
	return m_ParticleOutput[y * m_Width + x].Type;
}

void ParticleEmmiter::moveCell(int srcx, int srcy, int destx, int desty)
{
	m_FutureMovement.emplace_back(getIndex(srcx, srcy), getIndex(destx, desty));
}

void ParticleEmmiter::swapCells(int x, int y, int destx, int desty)
{
	auto part = m_ParticleOutput[getIndex(x, y)];
	m_ParticleOutput[getIndex(x, y)] = m_ParticleOutput[getIndex(destx, desty)];
	m_ParticleOutput[getIndex(destx, desty)] = part;
}

ParticleEmmiter::Particle& ParticleEmmiter::getCell(int x, int y)
{
	return m_ParticleOutput[y * m_Width + x];
}

void ParticleEmmiter::SandUpdate(int x, int y)
{
	getCell(x, y).Updated = true;
	if (getType(x, y - 1) == ParticleType::None)
		moveCell(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Smoke)
		swapCells(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Lava)
	{
		getCell(x, y).Type = ParticleType::Smoke;

		if (Random::Float() > 0.6f)
			getCell(x, y - 1).Type = ParticleType::None;
	}

	else if (getType(x, y - 1) == ParticleType::Water)
	{
		if (Random::Float() > 0.5f)
			swapCells(x, y, x, y - 1);
	}

	else if (getType(x + 1, y - 1) == ParticleType::None)
		moveCell(x, y, x + 1, y - 1);

	else if (getType(x + 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x + 1, y - 1).Type = ParticleType::None;
	}

	else if (getType(x + 1, y - 1) == ParticleType::Water)
		swapCells(x, y, x + 1, y - 1);

	else if (getType(x - 1, y - 1) == ParticleType::None)
		moveCell(x, y, x - 1, y - 1);

	else if (getType(x - 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x - 1, y - 1).Type = ParticleType::None;
	}

	else if (getType(x - 1, y - 1) == ParticleType::Water)
		swapCells(x, y, x - 1, y - 1);

}

void ParticleEmmiter::WaterUpdate(int x, int y)
{
	getCell(x, y).Updated = true;
	if (getType(x, y - 1) == ParticleType::None)
		moveCell(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Smoke)
		swapCells(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Lava)
	{
		getCell(x, y).Type = ParticleType::Smoke;

		if(Random::Float() > 0.6f)
			getCell(x, y - 1).Type = ParticleType::None;
	}

	else if (getType(x + 1, y - 1) == ParticleType::None)
		moveCell(x, y, x + 1, y - 1);
	
	else if (getType(x + 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x + 1, y - 1).Type = ParticleType::None;
	}

	else if (getType(x - 1, y - 1) == ParticleType::None)
		moveCell(x, y, x - 1, y - 1);

	else if (getType(x - 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x - 1, y - 1).Type = ParticleType::None;
	}

	else
	{
		bool left = false, right = false;
		if (getType(x - 1, y) == ParticleType::None)
			left = true;

		else if (getType(x - 1, y) == ParticleType::Smoke)
			left = true;

		if (getType(x + 1, y) == ParticleType::None)
			right = true;

		else if (getType(x + 1, y) == ParticleType::Smoke)
			right = true;

		if (left && right)
		{
			if (Random::Float() > 0.5f)
				right = false;
			else left = false;
		}

		if (right)
			moveCell(x, y, x + 1, y);
		else if (left) moveCell(x, y, x - 1, y);
	}
}

void ParticleEmmiter::LavaUpdate(int x, int y)
{
	getCell(x, y).Updated = true;
	if (getType(x, y - 1) == ParticleType::None)
		moveCell(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Smoke)
		swapCells(x, y, x, y - 1);

	else if (getType(x, y - 1) == ParticleType::Water)
	{
		getCell(x, y).Type = ParticleType::Smoke;
		getCell(x, y - 1).Type = ParticleType::Rock;
	}

	else if (getType(x + 1, y - 1) == ParticleType::None)
		moveCell(x, y, x + 1, y - 1);

	else if (getType(x + 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x + 1, y - 1).Type = ParticleType::None;
	}

	else if (getType(x - 1, y - 1) == ParticleType::None)
		moveCell(x, y, x - 1, y - 1);

	else if (getType(x - 1, y - 1) == ParticleType::Smoke)
	{
		moveCell(x, y, x, y - 1);
		getCell(x - 1, y - 1).Type = ParticleType::None;
	}

	else
	{
		bool left = false, right = false;
		if (getType(x - 1, y) == ParticleType::None)
			left = true;

		else if (getType(x - 1, y) == ParticleType::Smoke)
			left = true;

		if (getType(x + 1, y) == ParticleType::None)
			right = true;

		else if (getType(x + 1, y) == ParticleType::Smoke)
			right = true;

		if (left && right)
		{
			if (Random::Float() - 0.5f >= 0)
				right = false;
			else left = false;
		}

		if (right)
			moveCell(x, y, x + 1, y);
		else if (left) moveCell(x, y, x - 1, y);
	}
}

void ParticleEmmiter::SmokeUpdate(int x, int y)
{
	getCell(x, y).Updated = true;
	if (getType(x, y + 1) != ParticleType::Smoke && getType(x, y + 1) != ParticleType::Stone)
		moveCell(x, y, x, y + 1);

	else if (getType(x + 1, y + 1) != ParticleType::Smoke && getType(x + 1, y + 1) != ParticleType::Stone)
		moveCell(x, y, x + 1, y + 1);

	else if (getType(x - 1, y + 1) != ParticleType::Smoke && getType(x - 1, y + 1) != ParticleType::Stone)
		moveCell(x, y, x - 1, y + 1);

	else
	{
		bool left = false, right = false;
		if (getType(x - 1, y) != ParticleType::Smoke && getType(x - 1, y) != ParticleType::Stone)
			left = true;

		if (getType(x + 1, y) != ParticleType::Smoke && getType(x + 1, y) != ParticleType::Stone)
			right = true;

		if (left && right)
		{
			if (Random::Float() - 0.5f >= 0)
				right = false;
			else left = false;
		}

		if (right)
			moveCell(x, y, x + 1, y);
		else if (left) moveCell(x, y, x - 1, y);
	}
}
