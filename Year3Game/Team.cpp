#include "Team.h"
#include "SoccerMatch.h"

#include <Year3Engine\Transformations.h>
#include <Year3Engine\MessageDispatcher.h>
#include "SoccerMessages.h"
#include "TeamStates.h"
#include "SteeringBehaviors.h"
#include "FieldPlayer.h"
#include "FieldPlayerStates.h"
#include "GoalKeeper.h"
#include "GoalKeeperStates.h"

const float MaxFloat  = (std::numeric_limits<float>::max)();

Team::Team(SoccerMatch* match, bool human, bool home, std::string name, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	:match(match),
	human(human),
	home(home),
	closestPlayer(NULL),
	controllingPlayer(NULL),
	supportingPlayer(NULL),
	receiver(NULL),
	score(0),
	GameComponent(name)
{
	// Store the team color
	teamColor[0] = r;
	teamColor[1] = g;
	teamColor[2] = b;
	teamColor[3] = a;

	stateMachine = new StateMachine<Team>(this);

	stateMachine->SetCurrentState(PrepareForKickOff::Instance());
	stateMachine->SetPreviousState(PrepareForKickOff::Instance());
	stateMachine->SetGlobalState(NULL);

	supportSpotCalc = new SupportSpotCalculator(6, 13, this);
}


Team::~Team(void)
{
	delete stateMachine;
	delete supportSpotCalc;
}

void Team::LoadContent()
{
	if(home)
	{
		// GoalKeeper
		std::tr1::shared_ptr<GoalKeeper> keeper(new GoalKeeper(this, 6, ReturnHome::Instance(), false, 0, 500));
		keeper->player2 = false;
		players.push_back(keeper);
		match->allPlayers.push_back(keeper);
		parent->AddComponent(keeper);

		std::tr1::shared_ptr<FieldPlayer> player2(new FieldPlayer(this, 7, Wait::Instance(), Player::defender, false, 200, 300));
		player2->player2 = false;
		players.push_back(player2);
		match->allPlayers.push_back(player2);
		parent->AddComponent(player2);

		std::tr1::shared_ptr<FieldPlayer> player3(new FieldPlayer(this, 13, Wait::Instance(), Player::defender, false, -200, 300));
		player3->player2 = false;
		players.push_back(player3);
		match->allPlayers.push_back(player3);
		parent->AddComponent(player3);

		std::tr1::shared_ptr<FieldPlayer> player4(new FieldPlayer(this, 1, Wait::Instance(), Player::attacker, false, 18, 0));
		player4->player2 = false;
		players.push_back(player4);
		match->allPlayers.push_back(player4);
		parent->AddComponent(player4);

		std::tr1::shared_ptr<FieldPlayer> player5(new FieldPlayer(this, 8, Wait::Instance(), Player::attacker, false, -30, 0));
		player5->player2 = false;
		players.push_back(player5);
		match->allPlayers.push_back(player5);
		parent->AddComponent(player5);

	}
	else
	{
		// GoalKeeper
		std::tr1::shared_ptr<GoalKeeper> keeper(new GoalKeeper(this, 11, ReturnHome::Instance(), false, 0, -500));
		keeper->player2 = true;
		players.push_back(keeper);
		match->allPlayers.push_back(keeper);
		keeper->Steering()->SeparationOn();
		parent->AddComponent(keeper);

		std::tr1::shared_ptr<Player> player2(new FieldPlayer(this, 10, Wait::Instance(), Player::defender, false, 200, -300));
		player2->player2 = true;
		players.push_back(player2);
		match->allPlayers.push_back(player2);
		player2->Steering()->SeparationOn();
		parent->AddComponent(player2);

		std::tr1::shared_ptr<Player> player3(new FieldPlayer(this, 16, Wait::Instance(), Player::defender, false, -200, -300));
		player3->player2 = true;
		players.push_back(player3);
		match->allPlayers.push_back(player3);
		player3->Steering()->SeparationOn();
		parent->AddComponent(player3);

		std::tr1::shared_ptr<Player> player4(new FieldPlayer(this, 9, Wait::Instance(), Player::attacker, false, 150, -100));
		player4->player2 = true;
		players.push_back(player4);
		match->allPlayers.push_back(player4);
		player4->Steering()->SeparationOn();
		parent->AddComponent(player4);

		std::tr1::shared_ptr<Player> player5(new FieldPlayer(this, 4, Wait::Instance(), Player::attacker, false, -150, -100));
		player5->player2 = true;
		players.push_back(player5);
		match->allPlayers.push_back(player5);
		player5->Steering()->SeparationOn();
		parent->AddComponent(player5);
	}

	stateMachine->CurrentState()->Enter(this);
}

void Team::Update(float deltaTime)
{
	CalcClosestPlayerToBall();

	//the team state machine switches between attack/defense behavior. It
	//also handles the 'kick off' state where a team must return to their
	//kick off positions before the whistle is blown
	stateMachine->Update();
}

void Team::Reset()
{
	for (playerIter = players.begin(); playerIter != players.end(); playerIter++)
	{
		(*playerIter)->Reset(); 
	}
}

//--------------------- ReturnAllFieldPlayersToHome ---------------------------
//
//  sends a message to all players to return to their home areas forthwith
//------------------------------------------------------------------------
void Team::ReturnAllFieldPlayersToHome()
{

	for (playerIter = players.begin(); playerIter != players.end(); ++playerIter)
	{
		if ((*playerIter)->Role() != Player::goal_keeper)
		{
			Dispatcher->DispatchMsg(NULL, (*playerIter).get(), Msg_GoHome, NULL);
		}
	}
}

void Team::CalcClosestPlayerToBall()
{
	float32 closest = std::numeric_limits<float>::max();

	for (playerIter = players.begin(); playerIter != players.end(); ++playerIter)
	{
		// Originally set to false
		(*playerIter)->isControlled = false;

		if((*playerIter)->PlayerRole() == Player::goal_keeper)
			continue;

		// Get the length of the vector between the player and ball
		double distance = DistanceSq((*playerIter)->GetPositionInPixels(), match->ball->GetPositionInPixels());

		(*playerIter)->SetDistSqToBall(distance);

		if(distance < closest)
		{
			closest = distance;
			closestPlayer = (*playerIter).get(); // store the closest player
		}
	}

	distSqToBallOfClosestPlayer = closest;
	closestPlayer->isControlled = true;
}

Player* Team::FindPlayerToPassTo(Player* passer)
{
	// Get the direction the player is facing and normailse it
	b2Vec2 passerFacing = passer->physicsObject.body->GetWorldVector(b2Vec2(1,0));
	passerFacing.Normalize();

	float closest = std::numeric_limits<float>::max();

	Player* receiver = NULL;

	for (playerIter = players.begin(); playerIter != players.end(); ++playerIter)
	{
		if((*playerIter).get() == passer)
			continue; // ignore the passer

		// Get the direction vector between the passer and this player and normalise
		b2Vec2 direction = ((*playerIter)->physicsObject.body->GetPosition() - match->ball->physicsObject.body->GetPosition());
		direction.Normalize();

		// Get the angle between the two vectors
		float32 dotProduct = b2Dot(passerFacing, direction);
		float angle = acos(dotProduct);

		if(angle < b2_pi / 4)
		{
			// Get the length of the vector between the player and ball
			float32 distance = ((*playerIter)->physicsObject.body->GetPosition() - match->ball->physicsObject.body->GetPosition()).LengthSquared();

			if(distance < closest)
			{
				closest = distance;
				receiver = (*playerIter).get(); // store the closest player
			}
		}
	}

	return receiver;
}

void Team::Pass(Player* passer)
{
	Player* receiver = FindPlayerToPassTo(passer);
	if(receiver != NULL)
	{
		Dispatcher->DispatchMsg(passer, receiver, Msg_ReceiveBall, &receiver->GetPositionInPixels());

		// Get the direction vector between the passer and the ball and normalise
		b2Vec2 direction = (receiver->physicsObject.body->GetPosition() - match->ball->physicsObject.body->GetPosition());
		direction.Normalize();
		match->ball->Pass(direction, passer->PassPower());
	}
}

//----------------------- isPassSafeFromOpponent -------------------------
//
//  test if a pass from 'from' to 'to' can be intercepted by an opposing
//  player
//------------------------------------------------------------------------
bool Team::isPassSafeFromOpponent(b2Vec2 from, b2Vec2 target, const Player* const receiver, 
	const Player* const opp, double PassingForce)const
{
	//move the opponent into local space.
	b2Vec2 ToTarget = target - from;
	ToTarget.Normalize();

	//b2Vec2 ToTargetNormalized = Vec2DNormalize(ToTarget);

	b2Vec2 LocalPosOpp = PointToLocalSpace(opp->GetPositionInPixels(),
		ToTarget,
		Perp(ToTarget),
		from);

	//if opponent is behind the kicker then pass is considered okay(this is 
	//based on the assumption that the ball is going to be kicked with a 
	//velocity greater than the opponent's max velocity)
	if ( LocalPosOpp.y < 0 )
	{     
		return true;
	}


	//if the opponent is further away than the target we need to consider if
	//the opponent can reach the position before the receiver.
	b2Vec2 oppPos = opp->GetPositionInPixels();
	if (DistanceSq(from, target) < DistanceSq(oppPos, from))
	{
		if(receiver)
		{
			b2Vec2 receiverPos = receiver->GetPositionInPixels();
			if( DistanceSq(target, oppPos) > DistanceSq(target, receiverPos))
			{
				return true;
			}

			else
			{
				return false;
			}

		}

		else
		{
			return true;
		} 
	}

	//calculate how long it takes the ball to cover the distance to the 
	//position orthogonal to the opponents position
	double TimeForBall = 
		match->ball->TimeToCoverDistance(b2Vec2(0,0),
		b2Vec2(LocalPosOpp.x, 0),
		PassingForce);

	//now calculate how far the opponent can run in this time
	double reach = opp->MaxSpeed() * TimeForBall + match->ball->Radius()+ opp->radius;

	//if the distance to the opponent's y position is less than his running
	//range plus the radius of the ball and the opponents radius then the
	//ball can be intercepted
	if ( fabs(LocalPosOpp.x) < reach )
	{
		return false;
	}

	return true;
}

//---------------------- isPassSafeFromAllOpponents ----------------------
//
//  tests a pass from position 'from' to position 'target' against each member
//  of the opposing team. Returns true if the pass can be made without
//  getting intercepted
//------------------------------------------------------------------------
bool Team::isPassSafeFromAllOpponents(b2Vec2 from,b2Vec2 target,
	const Player* const receiver, double PassingForce)const
{
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator opp = Opponents()->Members().begin();

	for (opp; opp != Opponents()->Members().end(); ++opp)
	{
		if (!isPassSafeFromOpponent(from, target, receiver, (*opp).get(), PassingForce))
		{
			//debug_on

			return false;
		}
	}

	return true;
}


//------------------------ CanShoot --------------------------------------
//
//  Given a ball position, a kicking power and a reference to a vector2D
//  this function will sample random positions along the opponent's goal-
//  mouth and check to see if a goal can be scored if the ball was to be
//  kicked in that direction with the given power. If a possible shot is 
//  found, the function will immediately return true, with the target 
//  position stored in the vector ShotTarget.
//------------------------------------------------------------------------
bool Team::CanShoot(b2Vec2 BallPos, double power, b2Vec2& ShotTarget)const
{
	//the number of randomly created shot targets this method will test 
	int NumAttempts = 5; // Prm.NumAttemptsToFindValidStrike;

	while (NumAttempts--)
	{
		//choose a random position along the opponent's goal mouth. (making
		//sure the ball's radius is taken into account)
		if(home) // bottom goal
		{
			ShotTarget = b2Vec2(0, match->pitch->topGoal->GetPositionInPixels().y);
			
			//the x value of the shot position should lay somewhere between two
			//goalposts (taking into consideration the ball diameter)
			int MinXVal = (int)(match->pitch->bottomGoal->LeftPost());
			int MaxXVal = (int)(match->pitch->bottomGoal->RightPost());

			ShotTarget.x = (double)RandInt(MinXVal, MaxXVal);
		}
		else
		{
			ShotTarget = b2Vec2(0, match->pitch->bottomGoal->GetPositionInPixels().y);

			//the x value of the shot position should lay somewhere between two
			//goalposts (taking into consideration the ball diameter)
			int MinXVal = (int)(match->pitch->topGoal->LeftPost());
			int MaxXVal = (int)(match->pitch->topGoal->RightPost());

			ShotTarget.x = (double)RandInt(MinXVal, MaxXVal);
		}

		//make sure striking the ball with the given power is enough to drive
		//the ball over the goal line.
		double time = match->ball->TimeToCoverDistance(BallPos, ShotTarget, power);

		//if it is, this shot is then tested to see if any of the opponents
		//can intercept it.
		if (time >= 0)
		{
			if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
			{
				return true;
			}
		}
	}

	return false;
}

bool Team::AllPlayersAtHome() const
{
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator it = players.begin();
	for (it; it != players.end(); it++)
	{
		if ((*it)->InHomeRegion() == false)
			return false;
	}

	return true;
}

void Team::SetPlayerHomeRegion(int player, int region)const
{
	assert ( (player>=0) && (player<(int)players.size()) );

	players[player]->SetHomeRegion(region);
}

//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void Team::UpdateTargetsOfWaitingPlayers()const
{
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator it = players.begin();
	for (it; it != players.end(); it++)
	{
		if ((*it)->Role() !=Player::goal_keeper)
		{
			//cast to a field player
			FieldPlayer* player = static_cast<FieldPlayer*>((*it).get());

			if ( player->GetFSM()->isInState(*Wait::Instance()) ||
				player->GetFSM()->isInState(*ReturnToHomeRegion::Instance()) )
			{
				player->Steering()->SetTarget(player->HomeRegion()->center);
			}
		}
	}
}

//------------- DetermineBestSupportingAttacker ------------------------
//
// calculate the closest player to the SupportSpot
//------------------------------------------------------------------------
Player* Team::DetermineBestSupportingAttacker()
{
	double ClosestSoFar = MaxFloat;

	Player* BestPlayer = NULL;

	std::vector<std::tr1::shared_ptr<Player>>::iterator it = players.begin();

	for (it; it != players.end(); ++it)
	{
		//only attackers utilize the BestSupportingSpot
		if ( ((*it)->Role() == Player::attacker) && ((*it).get() != controllingPlayer) )
		{
			//calculate the dist. Use the squared value to avoid sqrt
			double dist = DistanceSq((*it)->GetPositionInPixels(), supportSpotCalc->GetBestSupportingSpot());

			//if the distance is the closest so far and the player is not a
			//goalkeeper and the player is not the one currently controlling
			//the ball, keep a record of this player
			if ((dist < ClosestSoFar) )
			{
				ClosestSoFar = dist;

				BestPlayer = (*it).get();
			}
		}
	}

	return BestPlayer;
}

//----------------------------- isOpponentWithinRadius ------------------------
//
//  returns true if an opposing player is within the radius of the position
//  given as a parameter
//-----------------------------------------------------------------------------
bool Team::isOpponentWithinRadius(b2Vec2 pos, double rad)
{
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator end = Opponents()->Members().end();
	std::vector<std::tr1::shared_ptr<Player>>::const_iterator it;

	for (it=Opponents()->Members().begin(); it !=end; ++it)
	{
		if (DistanceSq(pos, (*it)->GetPositionInPixels()) < rad*rad)
		{
			return true;
		}
	}

	return false;
}

//-------------------------- FindPass ------------------------------
//
//  The best pass is considered to be the pass that cannot be intercepted 
//  by an opponent and that is as far forward of the receiver as possible
//------------------------------------------------------------------------
bool Team::FindPass(const Player*const passer, 
	Player*& receiver,
	b2Vec2& PassTarget,
	double power,
	double MinPassingDistance)const
{  

	std::vector<std::tr1::shared_ptr<Player>>::const_iterator curPlyr = Members().begin();

	double    ClosestToGoalSoFar = MaxFloat;
	b2Vec2 Target;

	//iterate through all this player's team members and calculate which
	//one is in a position to be passed the ball 
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		//make sure the potential receiver being examined is not this player
		//and that it is further away than the minimum pass distance
		if ( ((*curPlyr).get() != passer) &&            
			(DistanceSq(passer->GetPositionInPixels(), (*curPlyr)->GetPositionInPixels()) > 
			MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, curPlyr->get(), Target, power))
			{
				//if the pass target is the closest to the opponent's goal line found
				// so far, keep a record of it
				double Dist2Goal;

				if(home)
					Dist2Goal = fabs(Target.y - match->pitch->topGoal->GetPositionInPixels().y);
				else
					Dist2Goal = fabs(Target.y - match->pitch->topGoal->GetPositionInPixels().y);

				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;

					//keep a record of this player
					receiver = curPlyr->get();

					//and the target
					PassTarget = Target;
				}     
			}
		}
	}//next team member

	if (receiver) return true;

	else return false;
}

//---------------------- GetBestPassToReceiver ---------------------------
//
//  Three potential passes are calculated. One directly toward the receiver's
//  current position and two that are the tangents from the ball position
//  to the circle of radius 'range' from the receiver.
//  These passes are then tested to see if they can be intercepted by an
//  opponent and to make sure they terminate within the playing area. If
//  all the passes are invalidated the function returns false. Otherwise
//  the function returns the pass that takes the ball closest to the 
//  opponent's goal area.
//------------------------------------------------------------------------
bool Team::GetBestPassToReceiver(const Player* const passer,
	const Player* const receiver,
	b2Vec2& PassTarget,
	double power)const
{  
	//first, calculate how much time it will take for the ball to reach 
	//this receiver, if the receiver was to remain motionless 
	double time = match->ball->TimeToCoverDistance(match->ball->GetPositionInPixels(),
		receiver->GetPositionInPixels(),
		power);

	//return false if ball cannot reach the receiver after having been
	//kicked with the given power
	if (time < 0) return false;

	PassTarget = receiver->GetPositionInPixels();

	return isPassSafeFromAllOpponents(match->ball->GetPositionInPixels(), receiver->GetPositionInPixels(), receiver, power);
}

//------------------------- RequestPass ---------------------------------------
//
//  this tests to see if a pass is possible between the requester and
//  the controlling player. If it is possible a message is sent to the
//  controlling player to pass the ball asap.
//-----------------------------------------------------------------------------
void Team::RequestPass(FieldPlayer* requester)const
{
	//maybe put a restriction here
	if (RandFloat() > 0.1) return;

	if (isPassSafeFromAllOpponents(ControllingPlayer()->GetPositionInPixels(),
		requester->GetPositionInPixels(),
		requester,
		100))
	{

		//tell the player to make the pass
		//let the receiver know a pass is coming 
		Dispatcher->DispatchMsg(
			requester,
			ControllingPlayer(),
			Msg_PassToMe,
			requester); 
	}
}