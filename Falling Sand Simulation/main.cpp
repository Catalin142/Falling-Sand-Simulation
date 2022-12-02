#include "sandbox.h"

int main()
{
	sandbox* sim = new sandbox(L"Falling Sand Simulation", 800, 600, 3);
	sim->ctx->clear(0xFFFFFF);

	while (sim->is_running)
		sim->update();
}