#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModuleDebug.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	nBalls = 0;
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
	//font = App->fonts->Load();

	
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

	int Right_Wall[26] = {
	-45, -2,
	-46, 40,
	-34, 50,
	-33, 64,
	-45, 73,
	-45, 92,
	-76, 125,
	-76, 179,
	-46, 209,
	-46, 237,
	-46, 303,
	-22, 314,
	-21, 17
	};

	int Right_DownWall[16] = {
	236, 676,
	236, 688,
	397, 688,
	397, 651,
	346, 627,
	335, 618,
	306, 626,
	305, 636
	};

	int CenterWall[46] = {
	162, 218,
	234, 218,
	234, 172,
	238, 163,
	243, 158,
	253, 158,
	313, 218,
	313, 268,
	343, 284,
	305, 323,
	305, 459,
	290, 474,
	290, 356,
	282, 337,
	269, 322,
	252, 312,
	241, 310,
	224, 310,
	207, 314,
	197, 324,
	185, 336,
	154, 303,
	154, 232
	};
	int LeftPolyBumper[16] = {
	137, 562,
	154, 573,
	159, 572,
	160, 566,
	144, 520,
	140, 518,
	133, 525,
	133, 557
	};

	int RightPolyBumper[16] = {
		280, 567,
	284, 572,
	303, 564,
	308, 558,
	308, 527,
	300, 519
	};

	int LeftFlipperWall[18] = {
		101, 526,
	101, 569,
	109, 575,
	111, 580,
	166, 608,
	164, 615,
	108, 587,
	98, 587,
	98, 531
	};

	int RightFlipperWall[16] = {
		275, 606,
	275, 616,
	333, 587,
	342, 586,
	342, 526,
	339, 526,
	339, 568,
	330, 578
	};

	

	App->physics->CreateStaticChain(0,0,mapa_de_sonic,90);
	App->physics->CreateStaticChain(420, 300, Right_Wall, 26);
	App->physics->CreateStaticChain(0 , 0, Right_DownWall, 16);
	App->physics->CreateStaticChain(0, 0, CenterWall, 46);

	App->physics->CreateStaticChain(0, 0, LeftFlipperWall, 18);
	App->physics->CreateStaticChain(0, 0, RightFlipperWall, 16);
	
	// --- polygon bumpers---//
	App->physics->CreateStaticChain(0, 0, LeftPolyBumper, 16, 0.7f, 0.2f);
	App->physics->CreateStaticChain(0, 0, RightPolyBumper, 12, 0.7f, 0.2f);

	PhysBody* EggHead1 =  App->physics->CreateStaticCircle(234, 360, 25, 0.2f);
	PhysBody* EggHead2 = App->physics->CreateStaticCircle(71, 432, 25, 0.2f);

	bumper1 = new Bumper(309, 335, *App->physics);
	bumper2 = new Bumper(370,356, *App->physics);
	bumper2 = new Bumper(309, 389, *App->physics);
	


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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && nBalls == 0)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		circles.getLast()->data->listener = this;
		nBalls++;
	}


	if (App->debug->debug)
	{

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
			circles.getLast()->data->listener = this;
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10);
		}

		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			ball->body->GetFixtureList()->SetRestitution(0.7f);
			App->physics->restitution = 0.7f;
		}
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			ball->body->GetFixtureList()->SetRestitution(0.3f);
			App->physics->restitution = 0.3f;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			App->physics->leftFlipper->body->ApplyForceToCenter(b2Vec2(0, -20), true);
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			App->physics->rightFlipper->body->ApplyForceToCenter(b2Vec2(0, -20), true);
		}
	}

	App->renderer->Blit(map,0,0,&maprect);
	
	/*p2List_item<PhysBody*>* item = circles.getFirst();
	while (item != NULL)
	{
		int posx, posy;
		item->data->GetPosition(posx, posy);
		App->renderer->Blit(circle, posx, posy,NULL, 1, item->data->GetRotation());

		item = item->next;
	}*/
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);
}
