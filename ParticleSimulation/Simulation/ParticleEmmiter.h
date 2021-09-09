#pragma once
#include <vector>

#include "../Engine/System/ScreenBuffer.h"

enum class ParticleType : int
{
	None,
	Water,
	Sand,
	Wood,
};

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

	static ParticleType getType(int x, int y);
	static void moveCell(int srcx, int secy, int destx, int desty);
	static ParticleType& getCell(int x, int y);

private:
	static ParticleType* m_ParticleOutput;

	static int m_PoolSize;
	static int m_Width;
	static int m_Height;

private:
	ParticleEmmiter() = default;
	~ParticleEmmiter() = default;

	static void SandUpdate(int x, int y);
	static void WaterUpdate(int x, int y);
};
