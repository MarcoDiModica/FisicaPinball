#include "ModuleDebug.h"

#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

#include "ModuleSceneIntro.h"
#include "ModulePhysics.h"
#include "p2Point.h"
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
	capFps = false;
	return true;
}


update_status ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		debug = false;
	}

	if (debug)
	{

		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) grav = !grav;

		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) colliders = !colliders;

		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) capFps = !capFps;


		if (grav == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
			{
				App->physics->GRAVITY_Y = 3.0f;
				App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
			}
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				App->physics->GRAVITY_Y = -3.0f;
				App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
			}
		}
		else
		{
			App->physics->GRAVITY_Y = 2.0f;
			App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
		}

		if (capFps == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && targetFPS < 120)
				targetFPS += 10;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && targetFPS > 10)
				targetFPS -= 10;
		}
		else
		{
			targetFPS = 60;
		}

		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				// Get new mouse position and re-target mouse_joint there
				b2Vec2 mousePosition;
				mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
				mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
				App->physics->mouse_joint->SetTarget(mousePosition);
			}
		}
		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				// Tell Box2D to destroy the mouse_joint
				App->physics->world->DestroyJoint(App->physics->mouse_joint);

				App->physics->mouse_joint = nullptr;
				App->physics->mouse_body = nullptr;
			}
		}

	}

	if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr && !debug)
	{
		App->physics->world->DestroyJoint(App->physics->mouse_joint);

		App->physics->mouse_joint = nullptr;
		App->physics->mouse_body = nullptr;
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
	if (colliders==true)
	{
		for (b2Body* b = App->physics->world->GetBodyList(); b; b = b->GetNext())
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
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 50, 50);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 50, 50);
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
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 255, 0);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 0, 255, 0);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 0, 0, 255);
				}
				break;
				}

				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				{
					// test if the current body contains mouse position
					b2Vec2 p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
					if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
					{
						if (App->physics->mouse_joint == nullptr)
						{

						
							// If a body was selected we will attach a mouse joint to it
							// so we can pull it around

							//Create a mouse joint using mouse_joint class property

							// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
							App->physics->mouse_body = b;

							// Get current mouse position
							b2Vec2 mousePosition;
							mousePosition.x = p.x;
							mousePosition.y = p.y;

							// Define new mouse joint
							b2MouseJointDef def;
							def.bodyA = App->physics->world->CreateBody(&b2BodyDef());; // First body must be a static ground
							def.bodyB = App->physics->mouse_body; // Second body will be the body to attach to the mouse
							def.target = mousePosition; // The second body will be pulled towards this location
							def.dampingRatio = 0.5f; // Play with this value
							def.frequencyHz = 2.0f; // Play with this value
							def.maxForce = 200.0f * App->physics->mouse_body->GetMass(); // Play with this value

							// Add the new mouse joint into the World
							App->physics->mouse_joint = (b2MouseJoint*)App->physics->world->CreateJoint(&def);
						}
						else 
						{
							b2Vec2 mousePosition;
							mousePosition.x = p.x;
							mousePosition.y = p.y;
							App->physics->mouse_joint->SetTarget(mousePosition);
						}
					}

				}

			}
		}

		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				// Draw a red line between both anchor points
				App->renderer->DrawLine(METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().x), METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
			}
		}
	}

}