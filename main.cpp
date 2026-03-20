#include "EngineCore.h"

int main() {
	//TEST ENGINE
	EngineCore engine;
	engine.main_init();
	engine.processMainLoop();
	engine.shutdown();
}
