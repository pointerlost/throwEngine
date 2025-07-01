#include "core/main.h"

int main()
{
	auto engine = std::make_unique<core::Engine>();

	engine->initPointerObjects();
	
	engine->initResources();

	engine->run();

	return 0;
}