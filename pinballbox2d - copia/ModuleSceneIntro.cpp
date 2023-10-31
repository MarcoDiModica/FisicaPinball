#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	maprect = {0,0,436, 699};
	App->renderer->camera.x = App->renderer->camera.y = 0;

	map = App->textures->Load("pinball/mapa_de_sonic.png");
	circle = App->textures->Load("pinball/wheel.png"); 
	//box = App->textures->Load("pinball/crate.png");
	//rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	
	int mapa_de_sonic[90] = {
	349, 254,
	365, 262,
	379, 267,
	421, 292,
	422, 265,
	422, 41,
	14, 41,
	13, 689,
	202, 689,
	202, 677,
	138, 641,
	138, 628,
	129, 621,
	109, 622,
	98, 621,
	45, 651,
	45, 679,
	19, 679,
	17, 628,
	68, 600,
	68, 501,
	65, 486,
	44, 478,
	30, 466,
	16, 443,
	16, 420,
	23, 400,
	36, 388,
	49, 379,
	66, 374,
	80, 375,
	92, 378,
	108, 389,
	95, 370,
	106, 370,
	114, 362,
	116, 354,
	120, 347,
	118, 333,
	99, 301,
	99, 256,
	102, 248,
	103, 125,
	344, 125,
	344, 252
	};
	App->physics->CreateStaticChain(0,0,mapa_de_sonic,90);

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->physics->leftFlipperJoint->SetMotorSpeed(-40);
	}
	else
	{
		App->physics->leftFlipperJoint->SetMotorSpeed(40);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->physics->rightFlipperJoint->SetMotorSpeed(40);
	}
	else
	{
		App->physics->rightFlipperJoint->SetMotorSpeed(-40);
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}
	App->renderer->Blit(map,0,0,&maprect);


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}
