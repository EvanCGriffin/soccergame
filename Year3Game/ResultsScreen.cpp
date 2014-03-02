#include "ResultsScreen.h"
#include <Year3Engine\Engine.h>
#include <Year3Engine\Sprite.h>

#include "MainMenuScreen.h"

ResultsScreen::ResultsScreen(std::string winnerName, bool draw)
	:font("Content//Fonts//bankgthd.ttf")
{
	if(!draw)
		message = winnerName + " won the match!";
	else
		message = "DRAW!";
}


ResultsScreen::~ResultsScreen(void)
{
}

void ResultsScreen::LoadContent()
{
	// center the image
	std::tr1::shared_ptr<Sprite> background(new Sprite("Content//Sprites//Results.png", "results", enginePtr->ScreenWidth / 2, enginePtr->ScreenHeight / 2));

	AddComponent(background);

	// Set the default camera
	enginePtr->camera = new Camera();

	font.LoadContent();
	font.SetSize(36);
}

void ResultsScreen::HandleInput(Input input)
{
	if(input.IsNewKeyPress(Keys::KEY_SPACE))
	{
		enginePtr->AddScreen(new MainMenuScreen());
		enginePtr->RemoveScreen(this);
	}
}

void ResultsScreen::Draw()
{
	GameScreen::Draw();

	font.RenderText(message, 30, 445, 0, 0, 0);
}