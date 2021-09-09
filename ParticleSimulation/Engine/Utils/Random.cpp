#include "Random.h"

std::mt19937 Random::m_Generator;
std::uniform_real_distribution<float> Random::m_Number(0, 1);