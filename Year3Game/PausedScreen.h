#pragma once

#include <Year3Engine\GameScreen.h>
#include <Year3Engine\EngineFont.h>

class PausedScreen : public GameScreen
{
public:
	PausedScreen(GameScreen* screenToControl);
	~PausedScreen(void);

	void LoadContent();
	void HandleInput(Input input);
	void Draw();

private:
	EngineFont font;
	GameScreen* screenToControl;
};

