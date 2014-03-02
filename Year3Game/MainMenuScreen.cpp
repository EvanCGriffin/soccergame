#include "MainMenuScreen.h"
#include <Year3Engine\Engine.h>
#include <Year3Engine\Sprite.h>

#include "TestScreen.h"

MainMenuScreen::MainMenuScreen(void)
{
}


MainMenuScreen::~MainMenuScreen(void)
{
}

void MainMenuScreen::LoadContent()
{
	// center the image
	std::tr1::shared_ptr<Sprite> background(new Sprite("Content//Sprites//MainMenu.png", "mainMenu", enginePtr->ScreenWidth / 2, enginePtr->ScreenHeight / 2));

	AddComponent(background);

	// Set the default camera
	enginePtr->camera = new Camera();
}

void MainMenuScreen::HandleInput(Input input)
{
	if(input.IsNewKeyPress(Keys::KEY_1))
	{
		enginePtr->AddScreen(new TestScreen(1));
		enginePtr->RemoveScreen(this);
	}

	if(input.IsNewKeyPress(Keys::KEY_2))
	{
		enginePtr->AddScreen(new TestScreen(2));
		enginePtr->RemoveScreen(this);
	}

	if(input.IsNewKeyPress(Keys::KEY_0))
	{
		enginePtr->AddScreen(new TestScreen(0));
		enginePtr->RemoveScreen(this);
	}
}