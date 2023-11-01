#pragma once
#include "Module.h"
#include "Application.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
//#define GRAVITY_Y -5.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

class ModulePhysics ;



// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
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

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
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

	float restitution = 0.4f;

	

private:

	float GRAVITY_Y = -2.0f;
	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;

	friend class ModuleDebug;
};

