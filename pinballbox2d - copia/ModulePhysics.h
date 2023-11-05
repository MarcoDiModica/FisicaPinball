#pragma once
#include "Module.h"
#include "Application.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include "ModulePhysics.h"
#include <cmath>
#include "Boost.h"

#define GRAVITY_X 0.0f
//#define GRAVITY_Y -5.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies


enum ColliderType {
	Esmeralds,
	Boost,
	notEsmeralds,
	Ring,
	Cannon
};

class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	ColliderType cType = notEsmeralds;
	bool Active = true;
	bool collected = false;
public:
	int width, height;
	b2Body* body;
	b2Vec2 force = b2Vec2(0.0f, 0.0f);
	Module* listener;
	int points = 0;
};

class ModulePhysics ;

class BoostPad {
public:
	PhysBody* pBody = nullptr;

	Timer DownTime;
	Timer ActiveTime;

	int waitTime;
	
	SDL_Texture* texture = nullptr;

	BoostPad(PhysBody* mybod) {
		pBody = mybod;
		pBody->Active = false;
	}
	void Update() {

		if (pBody->Active == false && DownTime.ReadSec() > waitTime) {

			pBody->Active = true;
			ActiveTime.Start();
		}

		else if (pBody->Active == true && ActiveTime.ReadSec() > waitTime) {

			pBody->Active = false;
			DownTime.Start();
		}
	}
};

class ChaosEsmerald {

public:
	bool Active = false;

	PhysBody* pBody = nullptr;
	ModulePhysics* myPhysics;
	b2Vec2 position;
	int force = 40;
	b2Vec2 center;

	SDL_Texture* texture;
	SDL_Rect rect = { 0,0,0,0 };

	int currentTime = 0;
	int changeTime = 90;

	bool isMovingUp = true;

	ChaosEsmerald(ModulePhysics* Physics) {
	
		myPhysics = Physics;
	
	
	
	};

	void Update() {
		if (isMovingUp) {
			// Move up
			pBody->body->SetTransform(pBody->body->GetPosition() + b2Vec2(0.0f, 0.015f), 0.0f);
			currentTime++;

			
			if (currentTime >= changeTime) {
				isMovingUp = false;
				currentTime = 0.0f;
			}
		}
		else {
			// Move down
			pBody->body->SetTransform(pBody->body->GetPosition() - b2Vec2(0.0f, 0.015f), 0.0f);
			
			currentTime++;
			if (currentTime >= changeTime) {
				isMovingUp = true;
				currentTime = 0.0f;
			}
		}
	}
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener 
{
public:


	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, float Friction = 0.2f , float Restitution = 0.3f, b2BodyType myType = b2BodyType::b2_dynamicBody);
	PhysBody* ModulePhysics::CreateStaticCircle(int x, int y, int radius, float Friction = 0.2f, float Restitution = 0.3f);
	PhysBody* CreateCoin(int x, int y, int radius);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* ModulePhysics::CreateStaticChain(int x, int y, int* points, int size, float Restitution = 0, float Friction = 0.2f);
	ChaosEsmerald* ModulePhysics::CreateChaosEsmerald(int x, int y, ModulePhysics* Physics);
	BoostPad* ModulePhysics::CreateBoostPad(int x, int y, int* points, int size, ModulePhysics* Physics,b2Vec2 force, int waitTime = 2);
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//Flippers
	void FlipandoEstoy();

	int leftFlipperX = 175;
	int leftFlipperY = 615;
	int rightFlipperX = 265;
	int rightFlipperY = 615;
	int flipperWidth = 40;
	int flipperHeight = 10;
	PhysBody* leftFlipper;
	PhysBody* rightFlipper;
	PhysBody* leftFlipperAnchor;
	PhysBody* rightFlipperAnchor;
	SDL_Texture* flipTexture1;
	SDL_Texture* flipTexture2;
	SDL_Texture* a;

	float restitution = 0.4f;

	//Cañones
	void CreateCanon();

	//Cañón derecha
	int RightCanonX, RightCanonY;
	int canonWidth, canonHeight;
	b2Vec2 canonCenter = b2Vec2(RightCanonX + canonWidth / 2, RightCanonY + 10);

	PhysBody* RightCanon;
	PhysBody* RightCanonAnchor;
	SDL_Texture* RightCanonTexture;

	//Cañón arriba
	int UpCanonX, UpCanonY;
	b2Vec2 UpcanonCenter = b2Vec2(UpCanonX + canonWidth / 2, UpCanonY + 10);

	PhysBody* UpCanon;
	PhysBody* UpAnchor;
	SDL_Texture* UpCanonTexture;


	SDL_Texture* coin;
	//Falta anim.h



	

private:

	float GRAVITY_Y = -2.0f;
public:
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* mouse_body;
	b2Body* ground;

	friend class ModuleDebug;
};

