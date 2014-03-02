#pragma once
#include <Year3Engine\Sprite.h>
#include <Year3Engine\EngineFont.h>
#include <sstream>

#include "Team.h"

class Scoreboard : public Sprite
{
public:
	Scoreboard(Team* homeTeam, Team* awayTeam, int minsPerGame);
	~Scoreboard(void);

	void Update(float deltaTime);
	void LoadContent();
	void Draw();

	void SetGameOn();
	void SetGameOff() { gameOn = false; }
private:
	EngineFont font;
	Team* homeTeam;
	Team* awayTeam;
	std::stringstream stringStream;
	float currentTime;
	int displaySeconds;
	int displayMinutes;
	double realtimeSeconds;
	static const int NINETY_MINS_IN_SECS;
	bool gameOn;
};






