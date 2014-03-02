#include "FieldPlayer.h"
#include "SteeringBehaviors.h"
#include "FieldPlayerStates.h"
#include "Team.h"
#include "SoccerMatch.h"

FieldPlayer::FieldPlayer(Team* team, int homeRegion, State<FieldPlayer>* startState, player_role role, bool dummyPlayer, GLfloat x, GLfloat y)
	: Player(team, homeRegion, role, dummyPlayer, x, y)
{
	stateMachine = new StateMachine<FieldPlayer>(this);

	if(startState)
	{
		stateMachine->SetCurrentState(startState);
		stateMachine->SetPreviousState(startState);
		stateMachine->SetGlobalState(GlobalPlayerState::Instance());

		stateMachine->CurrentState()->Enter(this);
	}

	steering->SeparationOn();
}

FieldPlayer::~FieldPlayer(void)
{
	delete stateMachine;
}

void FieldPlayer::Update(float deltaTime)
{
	stateMachine->Update();

	//Only AI controlled players should use sterring values
	if((team->Human() && !isClosestTeamMemberToBall()) || !team->Human() || !team->match->GameOn())
	{
		steering->Calculate();

		velocity = steering->Force();
	}

	// Update the Player
	Player::Update(deltaTime);
}


bool FieldPlayer::HandleMessage(const Telegram& msg)
{
	// Pass on the message to the state machine
	return stateMachine->HandleMessage(msg);
}