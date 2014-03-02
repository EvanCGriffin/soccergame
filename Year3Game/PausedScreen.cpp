#include "PausedScreen.h"
#include <Year3Engine\Engine.h>
#include <Year3Engine\Sprite.h>

PausedScreen::PausedScreen(GameScreen* screenToControl)
	:font("Content//Fonts//bankgthd.ttf"),
	screenToControl(screenToControl)
{
}


PausedScreen::~PausedScreen(void)
{
}

void PausedScreen::LoadContent()
{
	screenToControl->paused = true;
	enginePtr->physicsEnabled = false;

	// center the image
	std::tr1::shared_ptr<Sprite> background(new Sprite("Content//Sprites//Gradient.png", "gradient", enginePtr->ScreenWidth / 2, enginePtr->ScreenHeight / 2));

	// Make it transparent
	background->drawColor[3] = 0.6f;

	AddComponent(background);

	// Set the default camera
	//enginePtr->camera = new Camera();

	font.LoadContent();
	font.SetSize(36);
}

void PausedScreen::HandleInput(Input input)
{
	if(input.IsNewKeyPress(Keys::KEY_SPACE))
	{
		enginePtr->RemoveScreen(this);
		screenToControl->paused = false;
		enginePtr->physicsEnabled = true;
	}
}

void PausedScreen::Draw()
{
	glPopMatrix();

	GameScreen::Draw();

	font.RenderText("PAUSED", 30, 445);

	glPushMatrix();

	glTranslatef(enginePtr->camera->positionX, enginePtr->camera->positionY, 0);
}