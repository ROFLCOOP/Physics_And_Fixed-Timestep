#include "Projectile_SimulationApp.h"

int main() {
	
	// allocation
	auto app = new Projectile_SimulationApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}