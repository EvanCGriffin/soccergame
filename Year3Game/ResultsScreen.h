#pragma once

#include <Year3Engine\GameScreen.h>
#include <Year3Engine\EngineFont.h>
#include "Team.h"

class ResultsScreen : public GameScreen
{
public:
	ResultsScreen(std::string winnerName, bool draw);
	~ResultsScreen(void);

	void LoadContent();
	void HandleInput(Input input);
	void Draw();

private:
	EngineFont font;
	std::string message;
};

