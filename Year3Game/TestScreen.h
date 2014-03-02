#pragma once

#include <Year3Engine\GameScreen.h>
#include "SoccerMatch.h"

class TestScreen : public GameScreen
{
public:
	TestScreen(int humanPlayers);
	~TestScreen(void);

	void LoadContent();
	void HandleInput(Input input);
	void Draw();
private:
	std::tr1::shared_ptr<SoccerMatch> match;
	int humanPlayers;
	bool paused;
};

