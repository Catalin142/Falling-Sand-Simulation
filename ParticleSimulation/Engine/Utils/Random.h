#pragma once
#include <random>

class Random
{
private:
	static std::mt19937 m_Generator;
	static std::uniform_real_distribution<float> m_Number;

public:
	static void Init()
	{
		Random::m_Generator.seed(std::random_device()());
	}

	static float Float()
	{
		return m_Number(m_Generator);
	}

	static std::mt19937& getGenerator()
	{
		return m_Generator;
	}
};
