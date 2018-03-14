#include "PhysicsSceneFixedTimeStepApp.h"
//#include <vld.h>

int main() {
	

	// allocation
	auto app = new PhysicsSceneFixedTimeStepApp();

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}