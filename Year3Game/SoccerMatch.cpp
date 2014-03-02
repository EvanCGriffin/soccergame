#include "SoccerMatch.h"
#include "TeamStates.h"
#include "Radar.h"
#include "ResultsScreen.h"
#include <Year3Engine\Engine.h>

SoccerMatch::SoccerMatch(int humanPlayers)
	:reset(false),
	gameOn(false),
	goalKeeperHasBall(false),
	humanPlayers(humanPlayers)
{
}


SoccerMatch::~SoccerMatch(void)
{
}

void SoccerMatch::LoadContent()
{
	ball.reset(new Ball(this));

	pitch.reset(new Pitch());
	parent->AddComponent(pitch);

	if(humanPlayers == 1) // Play against AI
	{
		homeTeam.reset(new Team(this, true, true, "RED", 1, 0, 0));
		parent->AddComponent(homeTeam);

		awayTeam.reset(new Team(this, false, false, "BLU",  0, 0, 1));
		parent->AddComponent(awayTeam);
	}
	else if(humanPlayers == 2) // Two player
	{
		homeTeam.reset(new Team(this, true,  true, "RED", 1, 0, 0));
		parent->AddComponent(homeTeam);

		awayTeam.reset(new Team(this, true, false, "BLU", 0, 0, 1));
		parent->AddComponent(awayTeam);
	}
	else // AI vs AI
	{
		homeTeam.reset(new Team(this, false,  true, "RED", 1, 0, 0));
		parent->AddComponent(homeTeam);

		awayTeam.reset(new Team(this, false, false, "BLU", 0, 0, 1));
		parent->AddComponent(awayTeam);
	}

	homeTeam->SetOpponents(&(*awayTeam));
	awayTeam->SetOpponents(&(*homeTeam));

	// Add the ball last so it draws on top of everything
	parent->AddComponent(ball);

	scoreboard.reset(new Scoreboard(&(*homeTeam), &(*awayTeam), 1));
	radar.reset(new Radar(&(*homeTeam), &(*awayTeam), &(*ball)));

	// Add UI components last
	parent->AddComponent(scoreboard);
	parent->AddComponent(radar);
}

void SoccerMatch::Update(float deltaTime)
{
	if(reset)
	{
		gameOn = false;
		ball->Reset();
		homeTeam->GetFSM()->ChangeState(PrepareForKickOff::Instance());
		awayTeam->GetFSM()->ChangeState(PrepareForKickOff::Instance());

		reset = false;
	}
}

void SoccerMatch::Score(bool homeTeamScored)
{
	reset = true;

	// Update scores
	if(homeTeamScored)
		homeTeam->score++;
	else
		awayTeam->score++;
}

void SoccerMatch::Shoot(Player* player)
{
	ball->Shoot(player->ShotPower());
}

bool SoccerMatch::BallIsFree() const
{ 
	if(ball->Owner() == NULL)
		return true;

	return false;
}

void SoccerMatch::SetGameOn() 
{ 
	// Start the ball in a random direction
	double x = RandFloat();
	double y = RandFloat();
	if(RandFloat() < 0.5)
		x *= -1;

	if(RandFloat() < 0.5)
		y *= -1;

	ball->Push(b2Vec2(x, y), 2);

	gameOn = true; 

	//start the timer
	scoreboard->SetGameOn();
}

void SoccerMatch::TimeUp()
{
	gameOn = false;

	if(homeTeam->score > awayTeam->score) // HomeTeam won
	{
		parent->enginePtr->AddScreen(new ResultsScreen(homeTeam->name, false));
		parent->enginePtr->RemoveScreen(parent);
	}
	else if(awayTeam->score > homeTeam->score) // AwayTeam won
	{
		parent->enginePtr->AddScreen(new ResultsScreen(awayTeam->name, false));
		parent->enginePtr->RemoveScreen(parent);
	}
	else // Draw
	{
		parent->enginePtr->AddScreen(new ResultsScreen("", true));
		parent->enginePtr->RemoveScreen(parent);
	}
}