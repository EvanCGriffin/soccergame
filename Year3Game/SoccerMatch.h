#pragma once

#include <Year3Engine\GameComponent.h>
#include <Year3Engine\EngineFont.h>

#include "Team.h"
#include "Ball.h"
#include "Pitch.h"
#include "Scoreboard.h"


// Used for collision filtering
enum collisionCategory 
{
	PLAYER =			0x0001,
	BOUNDARY =			0x0002,
	BALL =				0x0004,
	GOAL =				0x0008,
	TACKLED_PLAYER =	0x0010
};

class SoccerMatch : public GameComponent
{
public:
	SoccerMatch(int humanPlayers);
	~SoccerMatch(void);

	void LoadContent();
	void Update(float deltaTime);

	// When a goal is scored the match 
	// resets and the team's score increases
	void Score(bool homeTeam);
	void Shoot(Player* player);

	// used for steering behaviors
	std::vector<std::tr1::shared_ptr<Player>> allPlayers;

	std::tr1::shared_ptr<Team> homeTeam; // The team at home
	std::tr1::shared_ptr<Team> awayTeam; // The team away
	std::tr1::shared_ptr<Ball> ball; // The ball
	std::tr1::shared_ptr<Pitch> pitch; // The pitch

	bool  GameOn()const { return gameOn; }
	void  SetGameOn();
	void  SetGameOff() { gameOn = false; scoreboard->SetGameOff();}

	bool  GoalKeeperHasBall()const { return goalKeeperHasBall; }
	void  SetGoalKeeperHasBall(bool b) { goalKeeperHasBall = b; }

	bool BallIsFree() const;

	void TimeUp();
private:
	bool reset; // Should the match reset

	//true if the game is in play. Set to false whenever the players
	//are getting ready for kickoff
	bool gameOn;

	//true if a goal keeper has possession
	bool goalKeeperHasBall;

	// Create our components
	std::tr1::shared_ptr<Scoreboard> scoreboard;
	std::tr1::shared_ptr<Radar> radar;

	int humanPlayers;
};

