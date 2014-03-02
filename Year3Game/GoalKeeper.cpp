#include "GoalKeeper.h"
#include <Year3Engine\Utils.h>
#include "SteeringBehaviors.h"
#include "GoalKeeperStates.h"
#include "Team.h"
#include "SoccerMatch.h"



GoalKeeper::GoalKeeper(Team* team, int homeRegion, State<GoalKeeper>* startState, bool dummyPlayer, GLfloat x, GLfloat y)
	: Player(team, homeRegion, Player::goal_keeper, dummyPlayer, x, y)
{
	stateMachine = new StateMachine<GoalKeeper>(this);

	if(startState)
	{
		stateMachine->SetCurrentState(startState);
		stateMachine->SetPreviousState(startState);
		stateMachine->SetGlobalState(GlobalKeeperState::Instance());

		stateMachine->CurrentState()->Enter(this);
	}

	steering->SeparationOn();
}

GoalKeeper::~GoalKeeper(void)
{
	delete stateMachine;
}

bool GoalKeeper::BallWithinRangeForIntercept()const
{
	if(team->Home())
		return (DistanceSq(team->match->pitch->bottomGoal->GetPositionInPixels(), team->match->ball->GetPositionInPixels()) <=
		20000); // GoalKeeperInterceptRange
	else
		return (DistanceSq(team->match->pitch->topGoal->GetPositionInPixels(), team->match->ball->GetPositionInPixels()) <=
		20000); // GoalKeeperInterceptRange

}

bool GoalKeeper::TooFarFromGoalMouth()const
{
	return (DistanceSq(GetPositionInPixels(), GetRearInterposeTarget()) >
		20000); // GoalKeeperInterceptRangeSq
}

b2Vec2 GoalKeeper::GetRearInterposeTarget()const
{
	double yPosTarget;
	if(team->Home())
		yPosTarget = team->match->pitch->bottomGoal->GetPositionInPixels().y;
	else
		yPosTarget = team->match->pitch->topGoal->GetPositionInPixels().y;

	double xPosTarget = team->match->pitch->PitchArea()->center.x - 5 * 0.5 + (team->match->ball->GetPositionInPixels().x * 5) / team->match->pitch->PitchArea()->rect.w;

	return b2Vec2(xPosTarget, yPosTarget); 
}

void GoalKeeper::Update(float deltaTime)
{
	stateMachine->Update();

	steering->Calculate();

	velocity = steering->Force();

	Player::Update(deltaTime);

	// Get the direcion between the ball and keeper
	b2Vec2 dir =  team->match->ball->GetPositionInPixels() - GetPositionInPixels();
	dir.Normalize();

	// Override the rotation, with a dir looking at the ball
	ClampToDirection(dir);

}

bool GoalKeeper::HandleMessage(const Telegram& msg)
{
	return stateMachine->HandleMessage(msg);
}