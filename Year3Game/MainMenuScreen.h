#pragma once
#include <Year3Engine\GameScreen.h>
#include "TestScreen.h"

class MainMenuScreen : public GameScreen
{
public:
	MainMenuScreen(void);
	~MainMenuScreen(void);

	void LoadContent();
	void HandleInput(Input input);
};

