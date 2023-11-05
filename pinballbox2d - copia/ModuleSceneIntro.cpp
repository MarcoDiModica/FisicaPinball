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
#include "Boost.h"

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
	circle = App->textures->Load("pinball/sonic.png"); 

	coin = App->textures->Load("pinball/ring.png");

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	ring_fx = App->audio->LoadFx("pinball/Audios/ring.wav");
	dash_fx = App->audio->LoadFx("pinball/Audios/spindash.ogg");

	boing_fx = App->audio->LoadFx("pinball/Audios/boing.ogg");

	palanca_fx = App->audio->LoadFx("pinball/palanca.ogg");

	//font = App->fonts->Load();
	App->audio->PlayMusic("pinball/musik.ogg"); 
	boost_texture = App->textures->Load("pinball/boostpad.png");
	
	RingSpin.PushBack({ 0,0, 18,16 });
	RingSpin.PushBack({ 17,0,18,16 });
	RingSpin.PushBack({ 34,0,18,16 });
	RingSpin.PushBack({ 51,0,18,16 });
	RingSpin.PushBack({ 68,0,18,16 });
	RingSpin.PushBack({ 85,0,18,16 });
	RingSpin.PushBack({ 102,0,18,16 });
	RingSpin.PushBack({ 119,0,18,16 });
	RingSpin.PushBack({ 136,0,18,16 });
	RingSpin.PushBack({ 153,0,18,16 });
	RingSpin.PushBack({ 170,0,18,16 });
	RingSpin.PushBack({ 187,0,18,16 });
	RingSpin.PushBack({ 204,0,18,16 });
	RingSpin.PushBack({ 221,0,18,16 });
	RingSpin.PushBack({ 238,0,18,16 });
	RingSpin.PushBack({ 255,0,18,16 });

	RingSpark.PushBack({ 0,20,18,16 });
	RingSpark.PushBack({ 17,20,18,16 });
	RingSpark.PushBack({ 34,20,18,16 });
	RingSpark.PushBack({ 51,20,18,16 });
	RingSpark.PushBack({ 68,18,16 });
	RingSpark.PushBack({ 85,20,18,16 });
	RingSpark.loop = false;
	RingSpark.speed = 0.1f;

	noRing.PushBack({ 0,0,0,0 });

	RingAnim = &RingSpin;
	
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
	int Wall[16] = {
	424, 689,
	424, 512,
	424, 428,
	424, 301,
	424, 262,
	429, 260,
	435, 689,
	427, 697
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

	int CenterWall[58] = {
	
	309, 410,
	308, 462,
	289, 482,
	289, 363,
	283, 339,
	262, 318,
	241, 311,
	216, 314,
	200, 323,
	185, 339,
	153, 311,
	153, 232,
	162, 221,
	316, 225,
	316, 271,
	345, 290,
	309, 324,
	300, 327,
	295, 331,
	293, 346,
	308, 356,
	309, 372,
	296, 385,
	294, 397,
	302, 407

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

	int newBumper1[10] = {
	312, 269+5,
	343, 285+5,
	343, 277+5,
	314, 259+5,
	310, 265+5
	};

	int top_bumper[8] = {
	160, 225-5,
	273, 225-5,
	273, 220-5,
	164, 220-5
	};

	int newBumper2[8] = {
	344, 256-4,
	375, 272-4,
	381, 270-4,
	345, 250-4
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

	App->physics->CreateStaticChain(0, 0, Wall, 16);

	App->physics->CreateStaticChain(0, 0, LeftFlipperWall, 18);
	App->physics->CreateStaticChain(0, 0, RightFlipperWall, 16);
	
	// --- polygon bumpers---//
	PhysBody* leftBumper = App->physics->CreateStaticChain(0, 0, LeftPolyBumper, 16, 0.5f, 0.4f);
	PhysBody* rightBumper = App->physics->CreateStaticChain(0, 0, RightPolyBumper, 12, 0.5f, 0.4f);
	PhysBody* TopBumper1 = App->physics->CreateStaticChain(0, 0, newBumper1, 10, 0.7f, 0.4f);
	PhysBody* TopBumper2 = App->physics->CreateStaticChain(0, 0, newBumper2, 8, 0.7f, 0.4f);
	leftBumper->points = 74; rightBumper->points = 74; TopBumper1->points = 74; TopBumper2->points = 74;

	PhysBody* TopBumper = App->physics->CreateStaticChain(0, 0, top_bumper, 8, 1.0f, 0.4f);
	TopBumper->points = 74;

	PhysBody* EggHead1 =  App->physics->CreateStaticCircle(236, 364, 24, 0.2f);
	PhysBody* EggHead2 = App->physics->CreateStaticCircle(71, 432, 24, 0.2f);

	bumper1 = new Bumper(309, 340, *App->physics); bumper1->pBody->points = 50;
	bumper2 = new Bumper(372,360, *App->physics); bumper2->pBody->points =  50;
	bumper3 = new Bumper(309, 392, *App->physics); bumper3->pBody->points = 50;
	bumper4 = new Bumper(230, 185, *App->physics); bumper4->pBody->points = 50;
	bumper5 = new Bumper(290, 185, *App->physics); bumper5->pBody->points = 50;
	bumper6 = new Bumper(265, 150, *App->physics); bumper6->pBody->points = 50;
	bumper7 = new Bumper(185, 147, *App->physics); bumper7->pBody->points = 50;

	 coin1 = App->physics->CreateCoin(138, 178, 8); coin1->cType = ColliderType::Ring;
	 coin2 = App->physics->CreateCoin(128, 228, 8); coin2->cType = ColliderType::Ring;
	 coin3 = App->physics->CreateCoin(128, 278, 8); coin3->cType = ColliderType::Ring;
	 coin4 = App->physics->CreateCoin(148, 338, 8); coin4->cType = ColliderType::Ring;

	 coin5 = App->physics->CreateCoin(235, 405, 8); coin5->cType = ColliderType::Ring;
	 coin6 = App->physics->CreateCoin(268, 391, 8); coin6->cType = ColliderType::Ring;
	 coin7 = App->physics->CreateCoin(267, 345, 8); coin7->cType = ColliderType::Ring;
	 coin8 = App->physics->CreateCoin(233, 329, 8); coin8->cType = ColliderType::Ring;

	 coin9 = App->physics->CreateCoin( 95, 400, 8); coin9->cType = ColliderType::Ring;
	 coin10 = App->physics->CreateCoin(54, 396, 8); coin10->cType = ColliderType::Ring;
	 coin11 = App->physics->CreateCoin(34, 440, 8); coin11->cType = ColliderType::Ring;
	 coin12 = App->physics->CreateCoin(68, 470, 8); coin12->cType = ColliderType::Ring;
	int boostPadcoords[8] = {
	118, 320,
	150, 370,
	179, 350,
	146, 301
	};
	int boostpad2[8] = {
	195, 155,
	195, 195,
	277, 195,
	277, 155
	};

	BoostPad1 = App->physics->CreateBoostPad(0, 0, boostPadcoords, 8, App->physics, b2Vec2(-20, -100));
	BoostPad1->texture = boost_texture;
	BoostPad2 = App->physics->CreateBoostPad(0, 0, boostpad2, 8, App->physics,b2Vec2(-40,0));
	BoostPad2->texture = boost_texture;

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
		App->physics->leftFlipper->body->ApplyForceToCenter(b2Vec2(0, -40), true);
		//App->audio->PlayFx(palanca_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		App->physics->rightFlipper->body->ApplyForceToCenter(b2Vec2(0, -40), true);
		//App->audio->PlayFx(palanca_fx);
	}

	if (!App->debug->debug)
	{
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && availableBalls > 0 && nBalls == 0)
		{
			
			circles.add(App->physics->CreateCircle(410, 580, 10,1.5f));
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

	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//{
	//	ball->body->GetFixtureList()->SetRestitution(0.7f);
	//	App->physics->restitution = 0.7f;
	//}
	//if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	//{
	//	ball->body->GetFixtureList()->SetRestitution(0.3f);
	//	App->physics->restitution = 0.3f;
	//}

	RingLogic();

	App->renderer->Blit(App->physics->flipTexture1 , App->physics->leftFlipperX - 7, App->physics->leftFlipperY - 7, NULL, 0, App->physics->leftFlipper->body->GetAngle() * RADTODEG, 5, 8 /*alto de la imagen*/);
	App->renderer->Blit(App->physics->flipTexture2, App->physics->rightFlipperX - 36, App->physics->rightFlipperY - 10, NULL, 0, App->physics->rightFlipper->body->GetAngle() * RADTODEG, 36/*ancho de la imagen*/, 8 /*alto de la imagen*/);

	BoostPad1->Update();
	if (BoostPad1->pBody->Active == true) {
		SDL_Rect r = { 0,0,72,41 };
		App->renderer->Blit(boost_texture, 110, 316, &r, 1.0f, 60);
	}
	else {
		SDL_Rect r = { 72,0,72,41 };
		App->renderer->Blit(boost_texture, 110, 316, &r, 1.0f, 60);
	}
	BoostPad2->Update();
	if (BoostPad2->pBody->Active == true) {
		SDL_Rect r = { 0,0,72,41 };
		App->renderer->Blit(boost_texture, 214-15, 151, &r, 1.0f, -0);
	}
	else {
		SDL_Rect r = { 72,0,72,41 };
		App->renderer->Blit(boost_texture, 214-15, 151, &r, 1.0f, -0);
	}

	p2List_item<PhysBody*>* item = circles.getFirst();
	p2List_item<PhysBody*>* next_item;

	while (item != NULL)
	{
		SDL_Rect cRect = { 0,0,27,25 };
		int posx, posy;
		item->data->GetPosition(posx, posy);
		App->renderer->Blit(circle, posx, posy, &cRect, 1, item->data->GetRotation());

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

	//----Spawn Chaos Esmeral every 10 seconds---------//
	if (EsmeraldSpawnTimer.ReadSec() > 10) {

		if (ActiveEsmeralds < 7) {
			Esmeralds[ActiveEsmeralds]->pBody->Active = true;
			ActiveEsmeralds++;
			EsmeraldSpawnTimer.Start();
		}

	}

	// ----------Draw Active esmeralds-----------//
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


	if (bodyB->cType == ColliderType::Boost && bodyB->Active == true) {

		bodyA->body->ApplyForce(bodyB->force, b2Vec2(0, 0), true);
		App->audio->PlayFx(dash_fx);

	}



	if (bodyB->Active && bodyB->cType == ColliderType::Esmeralds) {

		int i = 0;
		App->player->score += 777;
		bodyB->Active = false;
		App->player->collectedEsmeralds++;
	
		if (App->player->collectedEsmeralds > 6) {
			App->player->score *= 2;
			circle = App->textures->Load("pinball/superSonic.png");
		}
		

	}

	if (bodyB->Active && bodyB->cType == ColliderType::Ring) {

		App->player->score += 100;
		bodyB->Active = false;
		RingSpark.Reset();
		RingSpark.loopCount = 0;
		App->audio->PlayFx(ring_fx);

	}

	if (bodyB->cType == ColliderType::Cannon || bodyA->cType == ColliderType::Cannon) {
		App->audio->PlayFx(boing_fx);
	}
}

void ModuleSceneIntro::Reload() {

	for (int i = 0; i < 7; ++i) {

		Esmeralds[i]->pBody->Active = false;
		ActiveEsmeralds = 0;

		
		
	}

	coin1->collected = false; coin1->Active = true;
	coin2->collected = false; coin2->Active = true;
	coin3->collected = false; coin3->Active = true;
	coin4->collected = false; coin4->Active = true;
	coin5->collected = false; coin5->Active = true;
	coin6->collected = false; coin6->Active = true;
	coin7->collected = false; coin7->Active = true;
	coin8->collected = false; coin8->Active = true;
	coin9->collected = false; coin9->Active = true;
	coin10->collected = false; coin10->Active = true;
	coin11->collected = false; coin11->Active = true;
	coin12->collected = false; coin12->Active = true;

}

void ModuleSceneIntro::RingLogic() {

	// Si, esto es un desastre, es lo mejor que se me ha ocurrido sin usar particulas, y probablemente deberian estar en una lista o algo

	RingAnim->Update();
	App->renderer->Blit(map, 0, 0, &maprect);
	if (coin1->Active) App->renderer->Blit(coin, 130, 170, &RingAnim->GetCurrentFrame());
	else if (coin1->collected == false) {
		RingAnim1 = &RingSpark;
		RingAnim1->Update();
		App->renderer->Blit(coin, 130, 170, &RingAnim1->GetCurrentFrame());
		if (RingAnim1->HasFinished()) {
			coin1->collected = true;
			RingAnim1 = &RingSpin;
		}

	}
	if (coin2->Active)App->renderer->Blit(coin, 120, 220, &RingAnim->GetCurrentFrame());
	else if (coin2->collected == false) {
		RingAnim2 = &RingSpark;
		RingAnim2->Update();
		App->renderer->Blit(coin, 120, 220, &RingAnim2->GetCurrentFrame());
		if (RingAnim2->HasFinished()) {
			coin2->collected = true;
			RingAnim2 = &RingSpin;
		}
	}
	if (coin3->Active)App->renderer->Blit(coin, 120, 270, &RingAnim->GetCurrentFrame());
	else if (coin3->collected == false) {
		RingAnim3 = &RingSpark;
		RingAnim3->Update();
		App->renderer->Blit(coin, 120, 270, &RingAnim3->GetCurrentFrame());
		if (RingAnim3->HasFinished()) {
			coin3->collected = true;
			RingAnim3 = &RingSpin;
		}
	}
	if (coin4->Active)App->renderer->Blit(coin, 140, 330, &RingAnim->GetCurrentFrame());
	else if (coin4->collected == false) {
		RingAnim4 = &RingSpark;
		RingAnim4->Update();
		App->renderer->Blit(coin, 140, 330, &RingAnim4->GetCurrentFrame());
		if (RingAnim4->HasFinished()) {
			coin4->collected = true;
			RingAnim4 = &RingSpin;
		}
	}
	if (coin5->Active) App->renderer->Blit(coin, 235 - 8, 405 - 8, &RingAnim->GetCurrentFrame());
	else if (coin5->collected == false) {
		RingAnim5 = &RingSpark;
		RingAnim5->Update();
		App->renderer->Blit(coin, 235 - 8, 405 - 8, &RingAnim5->GetCurrentFrame());
		if (RingAnim5->HasFinished()) {
			coin5->collected = true;
			RingAnim5 = &RingSpin;
		}
	}
	if (coin6->Active)App->renderer->Blit(coin, 268 - 8, 391 - 8, &RingAnim->GetCurrentFrame());
	else if (coin6->collected == false) {
		RingAnim6 = &RingSpark;
		RingAnim6->Update();
		App->renderer->Blit(coin, 268 - 8, 391 - 8, &RingAnim6->GetCurrentFrame());
		if (RingAnim6->HasFinished()) {
			coin6->collected = true;
			RingAnim6 = &RingSpin;
		}
	}
	if (coin7->Active)App->renderer->Blit(coin, 267 - 8, 345 - 8, &RingAnim->GetCurrentFrame());
	else if (coin7->collected == false) {
		RingAnim7 = &RingSpark;
		RingAnim7->Update();
		App->renderer->Blit(coin, 267 - 8, 345 - 8, &RingAnim7->GetCurrentFrame());
		if (RingAnim7->HasFinished()) {
			coin7->collected = true;
			RingAnim7 = &RingSpin;
		}
	}
	if (coin8->Active)App->renderer->Blit(coin, 233 - 8, 329 - 8, &RingAnim->GetCurrentFrame());
	else if (coin8->collected == false) {
		RingAnim8 = &RingSpark;
		RingAnim8->Update();
		App->renderer->Blit(coin, 233 - 8, 329 - 8, &RingAnim8->GetCurrentFrame());
		if (RingAnim8->HasFinished()) {
			coin8->collected = true;

			RingAnim8 = &RingSpin;
		}
	}
	if (coin9->Active)App->renderer->Blit(coin, 95-8, 400-8, &RingAnim->GetCurrentFrame());
	else if (coin9->collected == false) {
		RingAnim9 = &RingSpark;
		RingAnim9->Update();
		App->renderer->Blit(coin, 95 - 8, 400 - 8, &RingAnim9->GetCurrentFrame());
		if (RingAnim9->HasFinished()) {
			coin9->collected = true;

			RingAnim9 = &RingSpin;
		}
	}
	if (coin10->Active)App->renderer->Blit(coin, 54-8, 396-8, &RingAnim->GetCurrentFrame());
	else if (coin10->collected == false) {
		RingAnim10 = &RingSpark;
		RingAnim10->Update();
		App->renderer->Blit(coin, 54-8, 396-8, &RingAnim10->GetCurrentFrame());
		if (RingAnim10->HasFinished()) {
			coin10->collected = true;

			RingAnim10 = &RingSpin;
		}
	}
	if (coin11->Active)App->renderer->Blit(coin, 34-8, 440-8, &RingAnim->GetCurrentFrame());
	else if (coin11->collected == false) {
		RingAnim11 = &RingSpark;
		RingAnim11->Update();
		App->renderer->Blit(coin, 34-8, 440-8, &RingAnim11->GetCurrentFrame());
		if (RingAnim11->HasFinished()) {
			coin11->collected = true;

			RingAnim11 = &RingSpin;
		}
	}
	if (coin12->Active)App->renderer->Blit(coin, 68-8, 470-8, &RingAnim->GetCurrentFrame());
	else if (coin12->collected == false) {
		RingAnim12 = &RingSpark;
		RingAnim12->Update();
		App->renderer->Blit(coin, 68-8, 470-8, &RingAnim12->GetCurrentFrame());
		if (RingAnim12->HasFinished()) {
			coin12->collected = true;

			RingAnim12 = &RingSpin;
		}
	}
}