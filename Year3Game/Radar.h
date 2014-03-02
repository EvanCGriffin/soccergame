#pragma once
#include <Year3Engine\Sprite.h>
#include "Team.h"
#include "Ball.h"
#include "SoccerMatch.h"

// A radar of the pitch and players
class Radar : public Sprite
{
public:
	Radar(Team* homeTeam, Team* awayTeam, Ball* ball);
	~Radar(void);

	void LoadContent();
	void Draw();

private:
	Team* homeTeam;
	Team* awayTeam;
	Ball* ball;
	Sprite blip;
};




