#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

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
	int score = 0;
	int maxScore = 0;
	int prevScore = 0;

	int collectedEsmeralds = 0;

public:

};