#pragma once

#include <Box2D\Common\b2Math.h>
#include <vector>

class Player; // Forward Declaration
class Ball; // Forward Declaration
class Pitch;

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

class SteeringBehaviors
{
public:
	SteeringBehaviors(Player* agent, Pitch* world, Ball* ball);

	virtual ~SteeringBehaviors(){}

	b2Vec2 Calculate();

	//calculates the component of the steering force that is parallel
	//with the vehicle heading
	double ForwardComponent();

	//calculates the component of the steering force that is perpendicuar
	//with the vehicle heading
	double SideComponent();

	b2Vec2 Force() const { return steeringForce; }

	//renders visual aids and info for seeing how each behavior is
	//calculated
	void RenderInfo();
	void RenderAids();

	b2Vec2 Target() const { return target; }
	void SetTarget(const b2Vec2 t) { target = t; }

	double InterposeDistance() const{ return interposeDist; }
	void SetInterposeDistance( double d ){ interposeDist = d; }

	bool Tagged() const { return tagged; }
	void Tag() { tagged = true; }
	void UnTag() { tagged = false; }


	void SeekOn(){flags |= seek;}
	void ArriveOn(){flags |= arrive;}
	void PursuitOn(){flags |= pursuit;}
	void SeparationOn(){flags |= separation;}
	void InterposeOn(double d){flags |= interpose; interposeDist = d;}


	void SeekOff() { if(On(seek)) flags ^=seek;}
	void ArriveOff() { if(On(arrive)) flags ^=arrive;}
	void PursuitOff() { if(On(pursuit)) flags ^=pursuit;}
	void SeparationOff() { if(On(separation)) flags ^=separation;}
	void InterposeOff() { if(On(interpose)) flags ^=interpose;}


	bool SeekIsOn() { return On(seek); }
	bool ArriveIsOn() { return On(arrive); }
	bool PursuitIsOn() { return On(pursuit); }
	bool SeparationIsOn() { return On(separation); }
	bool InterposeIsOn() { return On(interpose); }

private:
	Player* player;
	Ball* ball;

	// the steering force created by the combined effect of all
	// the selected behaviors
	b2Vec2 steeringForce;

	// the current target (usually the ball or predicted ball position)
	b2Vec2 target;

	// the distance the player tries to interpose from the target
	double interposeDist;

	// multipliers. 
	double multSeparation;

	//how far it can 'see'
	double viewDistance;

	//binary flags to indicate whether or not a behavior should be active
	int flags;

	enum behavior_type
	{
		none               = 0x0000,
		seek               = 0x0001,
		arrive             = 0x0002,
		separation         = 0x0004,
		pursuit            = 0x0008,
		interpose          = 0x0010
	};

	//used by group behaviors to tag neighbours
	bool         tagged;

	//Arrive makes use of these to determine how quickly a vehicle
	//should decelerate to its target
	enum Deceleration{ slow = 3, normal = 2, fast = 1 };

	//this behavior moves the agent towards a target position
	b2Vec2 Seek(b2Vec2 target);

	//this behavior is similar to seek but it attempts to arrive 
	//at the target with a zero velocity
	b2Vec2 Arrive(b2Vec2 target, Deceleration decel);

	//This behavior predicts where its prey will be and seeks
	//to that location
	b2Vec2 Pursuit(const Ball* ball);

	b2Vec2 Separation();

	//this attempts to steer the agent to a position between the opponent
	//and the object
	b2Vec2 Interpose(const Ball* ball, b2Vec2 pos, 
		double DistFromTarget);


	//finds any neighbours within the view radius
	void FindNeighbours();


	//this function tests if a specific bit of m_iFlags is set
	bool On(behavior_type bt){ return (flags & bt) == bt; }

	bool AccumulateForce(b2Vec2 &sf, b2Vec2 ForceToAdd);

	b2Vec2 SumForces();

	//a vertex buffer to contain the feelers rqd for dribbling
	std::vector<b2Vec2> m_Antenna;	
};

