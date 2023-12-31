#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleTextures.h"
#include "ModuleDebug.h"
#include "ModuleSceneIntro.h"
#include "Boost.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ground = NULL;
	world = NULL;
	mouse_joint = NULL;
	mouse_body = NULL;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	// it may work now
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);
	
	FlipandoEstoy();
	CreateCanon();

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

void ModulePhysics::FlipandoEstoy()
{
	leftFlipper = CreateRectangle(leftFlipperX, leftFlipperY, flipperWidth, flipperHeight);
	leftFlipperAnchor = CreateCircle(leftFlipperX, leftFlipperY, 2);
	leftFlipperAnchor->body->SetType(b2_staticBody);

	b2RevoluteJointDef leftFlipperJointDef;

	leftFlipperJointDef.bodyA = leftFlipper->body;
	leftFlipperJointDef.bodyB = leftFlipperAnchor->body;
	leftFlipperJointDef.referenceAngle = 0 * DEGTORAD;
	leftFlipperJointDef.enableLimit = true;
	leftFlipperJointDef.lowerAngle = -30 * DEGTORAD;
	leftFlipperJointDef.upperAngle = 30 * DEGTORAD;
	leftFlipperJointDef.localAnchorA.Set(PIXEL_TO_METERS(-13), 0);
	leftFlipperJointDef.localAnchorB.Set(0, 0);
	b2RevoluteJoint* leftFlipperJoint = (b2RevoluteJoint*)world->CreateJoint(&leftFlipperJointDef);

	rightFlipper = CreateRectangle(rightFlipperX, rightFlipperY, flipperWidth, flipperHeight);
	rightFlipperAnchor = CreateCircle(rightFlipperX, rightFlipperY, 2);
	rightFlipperAnchor->body->SetType(b2_staticBody);

	b2RevoluteJointDef rightFlipperJointDef;

	rightFlipperJointDef.bodyA = rightFlipper->body;
	rightFlipperJointDef.bodyB = rightFlipperAnchor->body;
	rightFlipperJointDef.referenceAngle = 0 * DEGTORAD;
	rightFlipperJointDef.enableLimit = true;
	rightFlipperJointDef.lowerAngle = -30 * DEGTORAD;
	rightFlipperJointDef.upperAngle = 30 * DEGTORAD;
	rightFlipperJointDef.localAnchorA.Set(PIXEL_TO_METERS(13), 0);
	rightFlipperJointDef.localAnchorB.Set(0, 0);
	b2RevoluteJoint* rightFlipperJoint = (b2RevoluteJoint*)world->CreateJoint(&rightFlipperJointDef);

	flipTexture1 = App->textures->Load("pinball/fliptex.png");
	flipTexture2 = App->textures->Load("pinball/fliptex2.png");
}

void ModulePhysics::CreateCanon()
{
	RightCanonX = 400;
	RightCanonY = 680;
	canonWidth = 20;
	canonHeight = 20;
	RightCanon = CreateRectangle(RightCanonX, RightCanonY, canonWidth, canonHeight);

	// Create the canon anchor (a static body)
	int anchorX = RightCanonX + canonWidth / 2; // Adjust the anchor position
	int anchorY = RightCanonY - 5; // Slightly above the canon
	PhysBody* canonAnchor = CreateCircle(anchorX, anchorY, 2);
	canonAnchor->body->SetType(b2_staticBody);

	// Define the canon joint
	b2RevoluteJointDef canonJointDef;
	canonJointDef.bodyA = RightCanon->body;
	canonJointDef.bodyB = canonAnchor->body;
	canonJointDef.referenceAngle = 0;
	canonJointDef.enableLimit = true;
	canonJointDef.lowerAngle = 0 * DEGTORAD; // Adjust the angles as needed
	canonJointDef.upperAngle = 0 * DEGTORAD;
	canonJointDef.localAnchorA.Set(0, 0);
	canonJointDef.localAnchorB.Set(0, 0);
	canonAnchor->cType =ColliderType::Cannon;
	RightCanon->cType = ColliderType::Cannon;
	// Create the canon joint
	b2RevoluteJoint* canonJoint = (b2RevoluteJoint*)world->CreateJoint(&canonJointDef);

	/*UpCanonX = 245;
	UpCanonY = 437;
	UpCanon = CreateRectangle(UpCanonX, UpCanonY, canonWidth, canonHeight);

	int UpanchorX = UpCanonX + canonWidth / 2;
	int UpanchorY = UpCanonY - 5;
	PhysBody* UpcanonAnchor = CreateCircle(UpanchorX, UpanchorY, 2);
	UpcanonAnchor->body->SetType(b2_staticBody);

	b2RevoluteJointDef UpcanonJointDef;
	UpcanonJointDef.bodyA = UpCanon->body;
	UpcanonJointDef.bodyB = UpcanonAnchor->body;
	UpcanonJointDef.referenceAngle = 0;
	UpcanonJointDef.enableLimit = true;
	UpcanonJointDef.lowerAngle = 45 * DEGTORAD;
	UpcanonJointDef.upperAngle = 45 * DEGTORAD;
	UpcanonJointDef.localAnchorA.Set(0, 0);
	UpcanonJointDef.localAnchorB.Set(0, 0);

	b2RevoluteJoint* UpcanonJoint = (b2RevoluteJoint*)world->CreateJoint(&UpcanonJointDef);*/
		
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float Friction, float Restitution, b2BodyType myType)
{
	b2BodyDef body;
	body.type = myType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	fixture.restitution = Restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticCircle(int x, int y, int radius, float Friction, float Restitution)
{
	b2BodyDef body;
	body.type = b2BodyType::b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	fixture.restitution = Restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}


PhysBody* ModulePhysics::CreateCoin(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	pbody->body->SetGravityScale(0);

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

ChaosEsmerald* ModulePhysics::CreateChaosEsmerald(int x, int y, ModulePhysics* Physics) {

	ChaosEsmerald* Esmerald = new ChaosEsmerald(this);
	
	 Esmerald->pBody =  CreateRectangleSensor(x, y, 20, 20);
	 Esmerald->pBody->body->SetGravityScale(0.0f);

	return Esmerald;
}

PhysBody* ModulePhysics::CreateStaticChain(int x, int y, int* points, int size, float Restitution, float Friction )
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;

	fixture.restitution = Restitution; 
	fixture.friction = Friction;

	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

BoostPad* ModulePhysics::CreateBoostPad(int x, int y,int* points, int size, ModulePhysics* Physics,b2Vec2 force, int waitTime) {

	
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;

	fixture.isSensor = true;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;
	pbody->cType = ColliderType::Boost;
	pbody->force = force;
	BoostPad* boost = new BoostPad(pbody);
	
	boost->waitTime = waitTime;
	return boost;
}
// 
update_status ModulePhysics::PostUpdate()
{
	if (App->debug->debug)
	{




		//if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		//	debug = !debug;

		//if(!debug)
		//	return UPDATE_CONTINUE;

		//App->renderer->Blit(flipTexture1, leftFlipperX - 7, leftFlipperY - 7, NULL, 0, leftFlipper->body->GetAngle() * RADTODEG, 5, 8 /*alto de la imagen*/);
		//App->renderer->Blit(flipTexture2, rightFlipperX - 36, rightFlipperY - 10, NULL, 0, rightFlipper->body->GetAngle() * RADTODEG, 36/*ancho de la imagen*/, 8 /*alto de la imagen*/);

		// Bonus code: this will iterate all objects in the world and draw the circles
		// You need to provide your own macro to translate meters to pixels
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}



				// TODO 1: If mouse button 1 is pressed ...
				// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
				// test if the current body contains mouse position
			}

		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	// TODO 4: If the player releases the mouse button, destroy the joint

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

	if (physA != nullptr && physB != nullptr)
	{
		float impulseStrength = 2.0f;

		// Calculate the angle of collision
		b2Vec2 collisionNormal = contact->GetManifold()->localNormal;
		float collisionAngle = atan2(collisionNormal.y, collisionNormal.x);

		b2Vec2 collisionImpulse(cos(collisionAngle) * impulseStrength, sin(collisionAngle) * impulseStrength);

		// Apply the impulse to the dynamic bodies involved in the collision
		if (physA == RightCanon && physB->body->GetType() == b2_dynamicBody)
		{
			physB->body->ApplyLinearImpulse(collisionImpulse, physA->body->GetWorldCenter(), true);
		}
		if (physB == RightCanon && physA->body->GetType() == b2_dynamicBody)
		{
			physA->body->ApplyLinearImpulse(collisionImpulse, physB->body->GetWorldCenter(), true);
		}

		if (physA == UpCanon && physB->body->GetType() == b2_dynamicBody)
		{
			physB->body->ApplyLinearImpulse(collisionImpulse, physA->body->GetWorldCenter(), true);
		}
		if (physB == UpCanon && physA->body->GetType() == b2_dynamicBody)
		{
			physA->body->ApplyLinearImpulse(collisionImpulse, physB->body->GetWorldCenter(), true);
		}

	}
}