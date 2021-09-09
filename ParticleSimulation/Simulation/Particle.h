#pragma once
#include "../Engine/Utils/ColorConverter.h"

enum class ParticleType : int
{
	None,
	Water,
	Sand,
	Wood,
};

class Particle
{
	friend class ParticleEmmiter;
public:
	Particle() { }

	void Set(ParticleType type) { m_Type = type; }
	virtual void onUpdate(float deltaTime) { }

	bool isActive = true;
	void operator=(const Particle& other)
	{
		m_Type = other.m_Type;
	}

protected:
	ParticleType m_Type = ParticleType::None;
	float m_Speed = 100.0f;

};