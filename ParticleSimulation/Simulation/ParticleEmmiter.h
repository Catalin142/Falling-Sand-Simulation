#pragma once
#include <vector>

#include "Particle.h"
#include "../Engine/System/ScreenBuffer.h"

struct ParticleMovement
{
	int srcX;
	int srcY;

	int destX;
	int destY;
};

class ParticleEmmiter
{
public:
	static void Init(const std::shared_ptr<ScreenBuffer>& buffer);

	static void setCell(ParticleType type, int x, int y);
	static void Update(float deltaTime);

	static void setEmitSpeed(float speed) { m_Speed = speed; }
	static void setEmitSize(int size) { m_Size = size; }

	static ParticleType getType(int x, int y);
	static void moveCell(int srcx, int secy, int destx, int desty);
	static void swapCells(int lx, int ly, int rx, int ry);
	static ParticleType& getCell(int x, int y);

private:
	static ParticleType* m_ParticleOutput;

	static int m_PoolSize;
	static int m_Width;
	static int m_Height;

	static float m_Speed;
	static int m_Size;

private:
	ParticleEmmiter() = default;
	~ParticleEmmiter() = default;

	static void SandUpdate(int x, int y, float deltaTime);
	static void WaterUpdate(int x, int y, float deltaTime);
	static void WoodUpdate(int x, int y, float deltaTime);
};
