#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include <string>
using namespace std;

#include <chrono>
using namespace std::chrono;

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled) 
{
	debug = false;
}

ModuleDebug::~ModuleDebug()
{

}

bool ModuleDebug::Start() 
{
	debug = false;
	return true;
}

update_status ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1)) debug = !debug;

	if (debug)
	{
		if(App->input->GetKey(SDL_SCANCODE_2)) grav = true;

		if (App->input->GetKey(SDL_SCANCODE_3)) colliders = true;
	}

	if (grav == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && App->physics->GRAVITY_Y < 20.0f)
		{
			App->physics->GRAVITY_Y += 1.0f;
			App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && App->physics->GRAVITY_Y > -20.0f)
		{
			App->physics->GRAVITY_Y -= 1.0f;
			App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
		}
	}

	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate()
{
	if (debug)
	{
		DebugDraw();
	}
	return UPDATE_CONTINUE;
}

void ModuleDebug::DebugDraw()
{

}