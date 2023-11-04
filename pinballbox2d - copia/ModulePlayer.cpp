#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz0123456789"};
	font = App->fonts->Load("pinball/sonic_font.png", lookupTable, 3);

	return true;
}



// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	sprintf_s(scoreText, 10, "%d", score);
	sprintf_s(prevScoreText, 10, "%d", prevScore);
	sprintf_s(maxScoreText, 10, "%d", maxScore);
	App->fonts->BlitText(180, 50, font, scoreText);
	App->fonts->BlitText(220, 20, font, "pts");
	App->fonts->BlitText(180, 70, font, "prev");
	App->fonts->BlitText(240, 70, font, prevScoreText);
	App->fonts->BlitText(180, 90, font, "max");
	App->fonts->BlitText(240, 90, font, maxScoreText);


	return UPDATE_CONTINUE;
}

void ModulePlayer::Loose() {

	if (maxScore < score) {
		maxScore = score;
	}

	prevScore = score;
	score = 0;
	collectedEsmeralds = 0;

}

