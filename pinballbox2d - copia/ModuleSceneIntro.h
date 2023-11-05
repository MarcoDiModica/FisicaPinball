#ifndef __MODULESCENEINTRO_H__
#define __MODULESCENEINTRO_H__

#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Timer.h"
#include "ModulePhysics.h"
#include "Animation.h"

#define DEGTORAD 0.0174532925199432957f

struct PhysBody;
struct PhysMotor;

class Bumper {
public:
	PhysBody* pBody = nullptr;

public:
	ModulePhysics* myPhysics;

	Bumper(int x, int y, ModulePhysics Physics) {

		this->myPhysics = &Physics;
		pBody = myPhysics->CreateCircle(x, y, 13, 0, 1.2f, b2BodyType::b2_staticBody);

	}

};




class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Reload();

	ChaosEsmerald* Esmeralds[7];
	b2Vec2 esmeraldsPositions[7];
	SDL_Texture* EsmeraldTexture;
	SDL_Texture* boost_texture;

	Timer EsmeraldSpawnTimer;
	uint ActiveEsmeralds;

	Animation* RingAnim;
	Animation RingSpin;

public:
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* coin;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* map;
	SDL_Rect maprect;
	uint bonus_fx;

	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	//---------Phyisical Bodies of the scene-----------//

	PhysBody* ball;

	Bumper* bumper1;
	Bumper* bumper2;
	Bumper* bumper3;
	Bumper* bumper4;
	Bumper* bumper5;
	Bumper* bumper6;
	Bumper* bumper7;

	BoostPad* BoostPad1;

	p2List_item<PhysBody*>* item = circles.getFirst();

	int font;
	int nBalls = 0;
	int availableBalls = 3;

};

#endif // __MODULESCENEINTRO_H__
