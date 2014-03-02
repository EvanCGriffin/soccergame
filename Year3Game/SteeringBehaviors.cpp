#include "SteeringBehaviors.h"

#include "Player.h"
#include "Team.h"
#include "Ball.h"
#include "SoccerMatch.h"

using std::string;
using std::vector;

//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehaviors::SteeringBehaviors(Player*  agent,
	Pitch* world,
	Ball*  ball):

player(agent),
	flags(0),
	multSeparation(100),
	tagged(false),
	viewDistance(1),
	ball(ball),
	interposeDist(0.0),
	m_Antenna(5, b2Vec2())
{
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehaviors::AccumulateForce(b2Vec2 &sf, b2Vec2 ForceToAdd)
{
	//first calculate how much steering force we have left to use
	double MagnitudeSoFar = sf.Length();

	double magnitudeRemaining = player->MaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (magnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.Length();

	//now calculate how much of the force we can really add  
	if (MagnitudeToAdd > magnitudeRemaining)
	{
		MagnitudeToAdd = magnitudeRemaining;
	}

	//add it to the steering force
	ForceToAdd.Normalize();
	ForceToAdd *= MagnitudeToAdd;
	sf += ForceToAdd; 

	return true;
}

//---------------------- Calculate ---------------------------------------
//
//  calculates the overall steering force based on the currently active
//  steering behaviors. 
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Calculate()
{                                                                         
	//reset the force
	steeringForce.SetZero();

	//this will hold the value of each individual steering force
	steeringForce = SumForces();

	//make sure the force doesn't exceed the vehicles maximum allowable
	double maxForce = player->MaxForce();

	Truncate(steeringForce, maxForce);

	return steeringForce;
}

//-------------------------- SumForces -----------------------------------
//
//  this method calls each active steering behavior and acumulates their
//  forces until the max steering force magnitude is reached at which
//  time the function returns the steering force accumulated to that 
//  point
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::SumForces()
{
	b2Vec2 force(0,0);

	//the soccer players must always tag their neighbors
	FindNeighbours();

	if (On(separation))
	{
		b2Vec2 separation = Separation();
		separation *= multSeparation;
		force += separation;

		if (!AccumulateForce(steeringForce, force)) 
			return steeringForce;
	}    

	if (On(seek))
	{
		force += Seek(target);

		if (!AccumulateForce(steeringForce, force)) 
			return steeringForce;
	}

	if (On(arrive))
	{
		force += Arrive(target, fast);

		if (!AccumulateForce(steeringForce, force)) 
			return steeringForce;
	}

	if (On(pursuit))
	{
		force += Pursuit(ball);

		if (!AccumulateForce(steeringForce, force)) 
			return steeringForce;
	}

	if (On(interpose))
	{
		force += Interpose(ball, target, interposeDist);

		if (!AccumulateForce(steeringForce, force)) 
			return steeringForce;
	}

	return steeringForce;
}

//------------------------- ForwardComponent -----------------------------
//
//  calculates the forward component of the steering force
//------------------------------------------------------------------------
double SteeringBehaviors::ForwardComponent()
{
	b2Vec2 facing = player->physicsObject.body->GetWorldVector(b2Vec2(1,0));
	return Dot(facing, steeringForce);
}

//--------------------------- SideComponent ------------------------------
//
//  //  calculates the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehaviors::SideComponent()
{
	return 1; // player->Side().Dot(m_vSteeringForce) * m_pPlayer->MaxTurnRate();
}


//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  allign the agent with the target and move the agent in the desired
//  direction
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Seek(b2Vec2 target)
{
	b2Vec2 dir = b2Vec2(target - player->GetPositionInPixels());
	dir.Normalize();
	dir *= player->MaxSpeed();

	return (dir - player->physicsObject.body->GetLinearVelocity());
}


//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Arrive(b2Vec2 target, Deceleration deceleration)
{
	b2Vec2 ToTarget = target - player->GetPositionInPixels();

	//calculate the distance to the target
	double dist = ToTarget.Length();

	if (dist > 0)
	{
		//because Deceleration is enumerated as an int, this value is required
		//to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.3;

		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed =  dist / ((double)deceleration * DecelerationTweaker);                    

		//make sure the velocity does not exceed the max
		speed = min(speed, player->MaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist.
		ToTarget *= speed;
		ToTarget *= (1 / dist);

		//b2Vec2 DesiredVelocity =  ToTarget * speed / dist;

		return (ToTarget - player->physicsObject.body->GetLinearVelocity());
	}

	return b2Vec2(0,0);
}


//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  ball
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Pursuit(const Ball* ball)
{
	b2Vec2 ToBall = ball->physicsObject.body->GetPosition() - player->physicsObject.body->GetPosition();

	//the lookahead time is proportional to the distance between the ball
	//and the pursuer; 
	double LookAheadTime = 0.0;

	if (ball->physicsObject.body->GetLinearVelocity().Length() != 0.0)
	{
		LookAheadTime = ToBall.Length() / ball->physicsObject.body->GetLinearVelocity().Length();
	}

	//calculate where the ball will be at this time in the future
	target = ball->FuturePosition(LookAheadTime);

	//now seek to the predicted future position of the ball
	return Arrive(target, fast);
}


//-------------------------- FindNeighbours ------------------------------
//
//  tags any vehicles within a predefined radius
//------------------------------------------------------------------------
void SteeringBehaviors::FindNeighbours()
{
	std::vector<std::tr1::shared_ptr<Player>>::iterator curPlyr;
	for (curPlyr = player->team->match->allPlayers.begin(); curPlyr!=player->team->match->allPlayers.end(); ++curPlyr)
	{
		//first clear any current tag
		(*curPlyr)->Steering()->UnTag();

		//work in distance squared to avoid sqrts
		b2Vec2 to = (*curPlyr)->GetPositionInPixels() - player->GetPositionInPixels();

		if (to.LengthSquared() < (viewDistance * viewDistance))
		{
			(*curPlyr)->Steering()->Tag();
		}
	}//next
	
}


//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Separation()
{  
	//iterate through all the neighbors and calculate the vector from the
	b2Vec2 SteeringForce(0,0);
	std::vector<std::tr1::shared_ptr<Player>>::iterator curPlyr;

	for (curPlyr = player->team->match->allPlayers.begin(); curPlyr!=player->team->match->allPlayers.end(); ++curPlyr)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent is close enough
		if((curPlyr->get() != player) && (*curPlyr)->Steering()->Tagged())
		{
			b2Vec2 ToAgent = player->GetPositionInPixels() - (*curPlyr)->GetPositionInPixels();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			double length = ToAgent.Length();
			ToAgent.Normalize();
			
			steeringForce += Divide(ToAgent, length);
		}
	}

	return SteeringForce;
}


//--------------------------- Interpose ----------------------------------
//
//  Given an opponent and an object position this method returns a 
//  force that attempts to position the agent between them
//------------------------------------------------------------------------
b2Vec2 SteeringBehaviors::Interpose(const Ball* ball, b2Vec2 target, double DistFromTarget)
{
	b2Vec2 dir(ball->GetPositionInPixels() - target);
	dir.Normalize();
	dir *= DistFromTarget;

	return Arrive(target + dir, normal);
}



