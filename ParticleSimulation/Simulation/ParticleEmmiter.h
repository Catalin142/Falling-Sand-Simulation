#pragma once
#include <vector>

#include "../Engine/System/ScreenBuffer.h"

enum class ParticleType : int
{
	None = 1,
	Sand,
	Water,
	Lava,
	Stone,
	Last,

	Smoke,
	Rock,

};

class ParticleEmmiter
{
	static struct Particle
	{
		ParticleType Type = ParticleType::None;
		bool Updated = false;

		void operator=(const Particle& other)
		{
			this->Type = other.Type;
			this->Updated = other.Updated;
		}
	};

	static struct ParticleMovement
	{
		int srcIndex, destIndex;
		ParticleMovement(int x, int y) : srcIndex(x), destIndex(y) {}
	};

public:
	static void Init(const std::shared_ptr<ScreenBuffer>& buffer);

	static void setCell(ParticleType type, int x, int y);
	static void Update(float deltaTime);

	static ParticleType getType(int x, int y);
	static void moveCell(int srcx, int secy, int destx, int desty);
	static void swapCells(int x, int y, int destx, int desty);
	static ParticleEmmiter::Particle& getCell(int x, int y);

private:
	static ParticleEmmiter::Particle* m_ParticleOutput;
	static std::vector<ParticleEmmiter::ParticleMovement> m_FutureMovement;

	static int m_PoolSize;
	static int m_Width;
	static int m_Height;

private:
	ParticleEmmiter() = default;
	~ParticleEmmiter() = default;

	static int getIndex(int x, int y) { return y * m_Width + x; }

	static void SandUpdate(int x, int y);
	static void WaterUpdate(int x, int y);
	static void LavaUpdate(int x, int y);
	static void SmokeUpdate(int x, int y);
};
