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
	capFps = false;
	return true;
}

update_status ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = true;
	}

	if (debug)
	{

		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) grav = !grav;

		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) colliders = !colliders;
		
		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN) capFps = !capFps;


		if (grav == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				App->physics->GRAVITY_Y = 3.0f;
				App->physics->world->SetGravity(b2Vec2(GRAVITY_X, App->physics->GRAVITY_Y));
			}
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
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
	if (colliders)
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
			}

			
		}
	}

}