#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"
#include <chrono>

using namespace std::chrono;

class ModuleDebug : public Module 
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);

	~ModuleDebug();

	bool Start() override;

	update_status Update();

	update_status PostUpdate();

	void DebugDraw();

public:
	microseconds elapsedCycle;
	microseconds elapsedFrame;
	int targetFPS = 60;
	double FPS;
	bool debug;

private:
	bool grav;
	bool capFps;
	bool colliders;
};

#endif // !__DEBUG_H__
