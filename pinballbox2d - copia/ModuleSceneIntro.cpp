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
#include "ModulePlayer.h"

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
	//box = App->textures->Load("pinball/a.png");
	coin = App->textures->Load("pinball/coin.png");
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
	130, 637,
	122, 632,
	109, 622,
	98, 621,
	91, 616,
	83, 614,
	78, 611,
	72, 601,
	68, 501,
	68, 486,
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
	EsmeraldTexture = App->textures->Load("pinball/chaos_esmeralds.png");
	int x = 0;
	esmeraldsPositions[0] = b2Vec2(137, 157);
	esmeraldsPositions[1] = b2Vec2(338, 358);
	esmeraldsPositions[2] = b2Vec2(310, 161);
	esmeraldsPositions[3] = b2Vec2(171, 357);
	esmeraldsPositions[4] = b2Vec2(225, 547);
	esmeraldsPositions[5] = b2Vec2(135, 417);
	esmeraldsPositions[6] = b2Vec2(192, 477);

	for (int i = 0; i < 7; ++i) {
		Esmeralds[i] = App->physics->CreateChaosEsmerald(  esmeraldsPositions[i].x, esmeraldsPositions[i].y, App->physics);
		Esmeralds[i]->texture = EsmeraldTexture;
	
		Esmeralds[i]->rect = { x,0,21,18 };
		x += 21;
		Esmeralds[i]->pBody->cType = ColliderType::Esmeralds;
		Esmeralds[i]->pBody->Active = false;
		Esmeralds[i]->pBody->points = 0;
	}
	


	App->physics->CreateStaticChain(0,0,mapa_de_sonic,90);
	App->physics->CreateStaticChain(420, 300, Right_Wall, 26);
	App->physics->CreateStaticChain(0 , 0, Right_DownWall, 16);
	App->physics->CreateStaticChain(0, 0, CenterWall, 46);

	App->physics->CreateStaticChain(0, 0, LeftFlipperWall, 18);
	App->physics->CreateStaticChain(0, 0, RightFlipperWall, 16);
	
	// --- polygon bumpers---//
	PhysBody* leftBumper = App->physics->CreateStaticChain(0, 0, LeftPolyBumper, 16, 0.7f, 0.2f);
	PhysBody* rightBumper = App->physics->CreateStaticChain(0, 0, RightPolyBumper, 12, 0.7f, 0.2f);
	leftBumper->points = 74; rightBumper->points = 74;

	PhysBody* EggHead1 =  App->physics->CreateStaticCircle(236, 364, 24, 0.2f);
	PhysBody* EggHead2 = App->physics->CreateStaticCircle(71, 432, 24, 0.2f);

	bumper1 = new Bumper(309, 340, *App->physics); bumper1->pBody->points = 50;
	bumper2 = new Bumper(372,360, *App->physics); bumper2->pBody->points =  50;
	bumper3 = new Bumper(309, 392, *App->physics); bumper3->pBody->points = 50;
	bumper4 = new Bumper(230, 185, *App->physics); bumper4->pBody->points = 50;
	bumper5 = new Bumper(290, 185, *App->physics); bumper5->pBody->points = 50;
	bumper6 = new Bumper(265, 150, *App->physics); bumper6->pBody->points = 50;
	bumper7 = new Bumper(185, 147, *App->physics); bumper7->pBody->points = 50;

	PhysBody* coin1 = App->physics->CreateCoin(138, 178, 8);
	PhysBody* coin2 = App->physics->CreateCoin(128, 228, 8);
	PhysBody* coin3 = App->physics->CreateCoin(128, 278, 8);
	PhysBody* coin4 = App->physics->CreateCoin(148, 338, 8);

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	EsmeraldSpawnTimer.Start();
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
	
	if (availableBalls == 0) {

		
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		App->physics->leftFlipper->body->ApplyForceToCenter(b2Vec2(0, -20), true);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		App->physics->rightFlipper->body->ApplyForceToCenter(b2Vec2(0, -20), true);
	}

	if (!App->debug->debug)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && availableBalls > 0 && nBalls == 0)
		{
			circles.add(App->physics->CreateCircle(410, 580, 10));
			circles.getLast()->data->listener = this;
			availableBalls--;
			nBalls++;
		}
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

	App->renderer->Blit(map,0,0,&maprect);
	App->renderer->Blit(coin, 130, 170);
	App->renderer->Blit(coin, 120, 220);
	App->renderer->Blit(coin, 120, 270);
	App->renderer->Blit(coin, 140, 330);
	App->renderer->Blit(App->physics->flipTexture1 , App->physics->leftFlipperX - 7, App->physics->leftFlipperY - 7, NULL, 0, App->physics->leftFlipper->body->GetAngle() * RADTODEG, 5, 8 /*alto de la imagen*/);
	App->renderer->Blit(App->physics->flipTexture2, App->physics->rightFlipperX - 36, App->physics->rightFlipperY - 10, NULL, 0, App->physics->rightFlipper->body->GetAngle() * RADTODEG, 36/*ancho de la imagen*/, 8 /*alto de la imagen*/);


	p2List_item<PhysBody*>* item = circles.getFirst();
	p2List_item<PhysBody*>* next_item;

	while (item != NULL)
	{
		int posx, posy;
		item->data->GetPosition(posx, posy);
		App->renderer->Blit(circle, posx, posy, NULL, 1, item->data->GetRotation());

		next_item = item->next;

		if (posy > SCREEN_HEIGHT)
		{
			circles.del(item);

			

			if (availableBalls == 0) {
				App->player->Loose();
				Reload();
				availableBalls = 3;
				LOG("You've lost");
			}

			nBalls--;
		}


		item = next_item; 
	}

	if (EsmeraldSpawnTimer.ReadSec() > 10) {

		if (ActiveEsmeralds < 7) {
			Esmeralds[ActiveEsmeralds]->pBody->Active = true;
			ActiveEsmeralds++;
			EsmeraldSpawnTimer.Start();
		}

	}


	for (int i = 0; i < 7; ++i) {
		if (Esmeralds[i]->pBody->Active == true) {

			Esmeralds[i]->Update();
			if (i == 0) {
				LOG("Here");
			}

			b2Vec2 pos = Esmeralds[i]->pBody->body->GetPosition();
			float x = pos.x;
			float y = pos.y;

			App->renderer->Blit(Esmeralds[i]->texture,
			METERS_TO_PIXELS(x)-10	, METERS_TO_PIXELS(y) -10, &Esmeralds[i]->rect);
		}
	}


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->player->score += bodyB->points;
	/*App->audio->PlayFx(bonus_fx);*/


	if (bodyB->points == 0) {
		
	} else if (bodyB-> points != 0) {

		App->audio->PlayFx(bonus_fx);
	}






	if (bodyB->Active && bodyB->cType == ColliderType::Esmeralds) {

		int i = 0;
		App->player->score += 777;
		bodyB->Active = false;
		App->player->collectedEsmeralds++;
	
		if (App->player->collectedEsmeralds > 6) {

		}
		

	}
}

void ModuleSceneIntro::Reload() {

	for (int i = 0; i < 7; ++i) {

		Esmeralds[i]->pBody->Active = true;
		ActiveEsmeralds = 0;

		circle = App->textures->Load("");
		
	}

}