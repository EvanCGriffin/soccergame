#include "FrameRateCounter.h"
#include <Year3Engine\Engine.h>

FrameRateCounter::FrameRateCounter(void)
	:GameComponent("FrameRateCounter"),
	font("Content//Fonts//arial.ttf"),
	frameRate(0),
	elapsedTime(0),
	frameCounter(0)
{
}


FrameRateCounter::~FrameRateCounter(void)
{
}

void FrameRateCounter::LoadContent()
{
	font.LoadContent();
}

void FrameRateCounter::Update(float deltaTime)
{
	elapsedTime += deltaTime;

	if (elapsedTime > 0.96) // 1 sec - 0.16 * 60
	{
		elapsedTime -= 0.96;
		frameRate = frameCounter;
		frameCounter = 0;
	}
}

void FrameRateCounter::Draw()
{
	frameCounter++;

	if(!parent->enginePtr->Debug())
		return;

	// Reset the string to blank
	stringStream.str("");
	// Convert int to string using stringstream
	stringStream << "Fps: " << frameRate;

	glPopMatrix();

	font.RenderText(stringStream.str(), 30, 20);

	glPushMatrix();

	glTranslatef(parent->enginePtr->camera->positionX, parent->enginePtr->camera->positionY, 0);
}
