#include "TestScreen.h"

#include <Year3Engine\Engine.h>

#include "BallCamera.h"
#include "SoccerMatch.h"
#include <Year3Engine\FrameRateCounter.h>
#include "MainMenuScreen.h"
#include "PausedScreen.h"

TestScreen::TestScreen(int humanPlayers)
	:GameScreen(),
	humanPlayers(humanPlayers)
{
}


TestScreen::~TestScreen(void)
{
	delete enginePtr->camera;
	enginePtr->camera = NULL;
}

void TestScreen::LoadContent()
{
	match.reset(new SoccerMatch(humanPlayers));
	AddComponent(match);

	// Add our camera
	enginePtr->camera = new BallCamera(this, &(*match->ball));

	enginePtr->soundManager->AddSound("Content//Sounds//bomb.wav", "Bomb");
	enginePtr->soundManager->AddMusic("Content//Sounds//song.wav", "Song");

	//enginePtr->soundManager->PlayMusic("Song", -1);

	std::tr1::shared_ptr<FrameRateCounter> frameRateCounter (new FrameRateCounter());
	AddComponent(frameRateCounter);

}

void TestScreen::HandleInput(Input input)
{
	if(input.IsNewKeyPress(Keys::KEY_ESCAPE))
	{
		enginePtr->AddScreen(new MainMenuScreen());
		enginePtr->RemoveScreen(this);
		return;
	}

	if(input.IsNewKeyPress(Keys::KEY_p))
	{
		enginePtr->AddScreen(new PausedScreen(this));
		return;
	}

	if(input.IsNewKeyPress(Keys::KEY_F1))
	{
		enginePtr->ToggleDebug();
	}

	GameScreen::HandleInput(input);
}

void TestScreen::Draw()
{
	GameScreen::Draw();

	// Draw debug data
	if(enginePtr->Debug())
	{
		// Disable Blending
		glDisable(GL_BLEND);

		match->pitch->Draw(&enginePtr->debugDraw);
		match->homeTeam->SupportSpotCalc()->Draw(&enginePtr->debugDraw);
		match->awayTeam->SupportSpotCalc()->Draw(&enginePtr->debugDraw);
	}
}
