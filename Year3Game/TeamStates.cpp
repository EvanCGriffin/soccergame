#include "TeamStates.h"
#include "Team.h"
#include "SoccerMatch.h"

void ChangePlayerHomeRegions(Team* team, const int NewRegions[5])
{
	for (int player=0; player<5; ++player)
	{
		team->SetPlayerHomeRegion(player, NewRegions[player]);
	}
}

PrepareForKickOff* PrepareForKickOff::Instance()
{
	static PrepareForKickOff instance;

	return &instance;
}

void PrepareForKickOff::Enter(Team* team)
{
	//reset key player pointers
	team->SetControllingPlayer(NULL);
	team->SetSupportingPlayer(NULL);
	team->SetReceiver(NULL);
	team->SetClosestPlayer(NULL);

	//send Msg_GoHome to each player.
	team->ReturnAllFieldPlayersToHome();
}

void PrepareForKickOff::Update(Team* team)
{
	// if both teams in position, start the game
	if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome())
	{
		team->GetFSM()->ChangeState(Defending::Instance());
	}
}

void PrepareForKickOff::Exit(Team* team)
{
	team->match->SetGameOn();
}

//************************************************************************ DEFENDING

Defending* Defending::Instance()
{
	static Defending instance;

	return &instance;
}


void Defending::Enter(Team* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Defending state" << "";
#endif

	//these define the home regions for this state of each of the players
	const int homeRegions[5] = {6,7,13,1,8};
	const int awayRegions[5] = {11,10,16,9,4};

	//set up the player's home regions
	if (team->Home())
	{
		ChangePlayerHomeRegions(team, homeRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, awayRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region
	team->UpdateTargetsOfWaitingPlayers();
}

void Defending::Update(Team* team)
{
	//if in control change states
	if (team->InControl())
	{
		team->GetFSM()->ChangeState(Attacking::Instance()); return;
	}
}


void Defending::Exit (Team* team) {}


//************************************************************************ ATTACKING

Attacking* Attacking::Instance()
{
	static Attacking instance;

	return &instance;
}


void Attacking::Enter(Team* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Attacking state" << "";
#endif

	//these define the home regions for this state of each of the players
	const int homeRegions[5] = { 6, 7, 14, 16, 4 };  // {1,12,14,6,4};
	const int awayRegions[5] = { 11, 10, 3, 13, 1 }; //{16,3,5,9,13};

	//set up the player's home regions
	if (team->Home())
	{
		ChangePlayerHomeRegions(team, homeRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, awayRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region to enable
	//it to move into the correct position.
	team->UpdateTargetsOfWaitingPlayers();
}


void Attacking::Update(Team* team)
{
	//if this team is no longer in control change states
	if (!team->InControl())
	{
		team->GetFSM()->ChangeState(Defending::Instance()); 
		return;
	}

	//calculate the best position for any supporting attacker to move to
	team->DetermineBestSupportingPosition();
}

void Attacking::Exit(Team* team)
{
	//there is no supporting player for defense
	team->SetSupportingPlayer(NULL);
}