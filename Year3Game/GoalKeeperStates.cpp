#include "GoalKeeperStates.h"

#include <Year3Engine/Telegram.h>
#include <Year3Engine/MessageDispatcher.h>
#include "SoccerMessages.h"
#include "GoalKeeper.h"
#include "SteeringBehaviors.h"
#include "Team.h"
#include "SoccerMatch.h"


//--------------------------- GlobalKeeperState -------------------------------
//-----------------------------------------------------------------------------

GlobalKeeperState* GlobalKeeperState::Instance()
{
	static GlobalKeeperState instance;

	return &instance;
}


bool GlobalKeeperState::OnMessage(GoalKeeper* keeper, const Telegram& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_GoHome:
		{
			keeper->SetDefaultHomeRegion();

			keeper->GetFSM()->ChangeState(ReturnHome::Instance());
		}

		break;

	case Msg_ReceiveBall:
		{
			keeper->GetFSM()->ChangeState(InterceptBall::Instance());
		}

		break;

	}//end switch

	return false;
}

//--------------------------- TendGoal -----------------------------------
//
//  This is the main state for the goalkeeper. When in this state he will
//  move left to right across the goalmouth using the 'interpose' steering
//  behavior to put himself between the ball and the back of the net.
//
//  If the ball comes within the 'goalkeeper range' he moves out of the
//  goalmouth to attempt to intercept it. (see next state)
//------------------------------------------------------------------------

TendGoal* TendGoal::Instance()
{
	static TendGoal instance;

	return &instance;
}


void TendGoal::Enter(GoalKeeper* keeper)
{
	//turn interpose on
	keeper->Steering()->InterposeOn(70.0);

	//interpose will position the agent between the ball position and a target
	//position situated along the goal mouth. This call sets the target
	keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

void TendGoal::Update(GoalKeeper* keeper)
{
	//the rear interpose target will change as the ball's position changes
	//so it must be updated each update-step 
	keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

	if(keeper->hasPossession)
	{
		keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());
		return;
	}

	//if ball is within a predefined distance, the keeper moves out from
	//position to try and intercept it.
	if (keeper->BallWithinRangeForIntercept() && !keeper->team->InControl())
	{
		keeper->GetFSM()->ChangeState(InterceptBall::Instance());
	}

	//if the keeper has ventured too far away from the goal-line and there
	//is no threat from the opponents he should move back towards it
	if (keeper->TooFarFromGoalMouth() && keeper->team->InControl())
	{
		keeper->GetFSM()->ChangeState(ReturnHome::Instance());

		return;
	}
}


void TendGoal::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->InterposeOff();
}


//------------------------- ReturnHome: ----------------------------------
//
//  In this state the goalkeeper simply returns back to the center of
//  the goal region before changing state back to TendGoal
//------------------------------------------------------------------------

ReturnHome* ReturnHome::Instance()
{
	static ReturnHome instance;

	return &instance;
}


void ReturnHome::Enter(GoalKeeper* keeper)
{
	keeper->Steering()->ArriveOn();
}

void ReturnHome::Update(GoalKeeper* keeper)
{
	keeper->Steering()->SetTarget(keeper->HomeRegion()->center);

	//if close enough to home or the opponents get control over the ball,
	//change state to tend goal
	if (keeper->InHomeRegion() || !keeper->team->InControl())
	{
		keeper->GetFSM()->ChangeState(TendGoal::Instance());
	}
}

void ReturnHome::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->ArriveOff();
}



//----------------- InterceptBall ----------------------------------------
//
//  In this state the GP will attempt to intercept the ball using the
//  pursuit steering behavior, but he only does so so long as he remains
//  within his home region.
//------------------------------------------------------------------------

InterceptBall* InterceptBall::Instance()
{
	static InterceptBall instance;

	return &instance;
}


void InterceptBall::Enter(GoalKeeper* keeper)
{
	keeper->Steering()->PursuitOn();  

#ifdef GOALY_STATE_INFO_ON
	debug_con << "Goaly " << keeper->ID() << " enters InterceptBall" <<  "";
#endif
}

void InterceptBall::Update(GoalKeeper* keeper)
{ 


	//if the goalkeeper moves to far away from the goal he should return to his
	//home region UNLESS he is the closest player to the ball, in which case,
	//he should keep trying to intercept it.
	if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
	{
		keeper->GetFSM()->ChangeState(ReturnHome::Instance());

		return;
	}

	if(keeper->hasPossession)
		keeper->GetFSM()->ChangeState(PutBallBackInPlay::Instance());
}

void InterceptBall::Exit(GoalKeeper* keeper)
{
	keeper->Steering()->PursuitOff();
}



//--------------------------- PutBallBackInPlay --------------------------
//
//------------------------------------------------------------------------

PutBallBackInPlay* PutBallBackInPlay::Instance()
{
	static PutBallBackInPlay instance;

	return &instance;
}

void PutBallBackInPlay::Enter(GoalKeeper* keeper)
{
	//let the team know that the keeper is in control
	keeper->team->SetControllingPlayer(keeper);
	keeper->team->match->SetGoalKeeperHasBall(true);

	//send all the players home
	keeper->team->Opponents()->ReturnAllFieldPlayersToHome();
	keeper->team->ReturnAllFieldPlayersToHome();
}


void PutBallBackInPlay::Update(GoalKeeper* keeper)
{
	Player*  receiver = NULL;
	b2Vec2   BallTarget;

	//test if there are players further forward on the field we might
	//be able to pass to. If so, make a pass.
	if (keeper->team->FindPass(keeper,
		receiver,
		BallTarget,
		50, //Prm.MaxPassingForce,
		10.0)) //Prm.GoalkeeperMinPassDist))
	{     
		//make the pass   
		b2Vec2 dir = BallTarget - keeper->team->match->ball->GetPositionInPixels();
		dir.Normalize();

		keeper->team->match->ball->Pass(dir, keeper->PassPower());

		//goalkeeper no longer has ball 
		keeper->team->match->SetGoalKeeperHasBall(false);

		//let the receiving player know the ball's comin' at him
		Dispatcher->DispatchMsg(keeper,
			receiver,
			Msg_ReceiveBall,
			&BallTarget);

		//go back to tending the goal   
		keeper->GetFSM()->ChangeState(TendGoal::Instance());

		return;
	}  

	keeper->physicsObject.body->SetLinearVelocity(b2Vec2(0,0));
}
