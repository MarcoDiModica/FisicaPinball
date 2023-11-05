#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "SDL/include/SDL.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Loose();

	int font = 20;
	char scoreText[10] = { "\0" };
	char prevScoreText[10] = { "\0" };
	char maxScoreText[10] = { "\0" };
	char currentLives[10] = { "\0" };
	int score = 0;
	int maxScore = 0;
	int prevScore = 0;

	int collectedEsmeralds = 0;
	bool isSuper = false;
	float pMultiplier = 1.0f;

	SDL_Texture* playerIcon = nullptr;
	SDL_Rect iconRect;

public:

};