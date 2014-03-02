#include "Ball.h"

#include <Year3Engine\CirclePhysicsObject.h>
#include <Year3Engine\Engine.h>
#include <Year3Engine\CirclePhysicsObject.h>
#include "SoccerMatch.h"


Ball::Ball(SoccerMatch* match, GLfloat positionX, GLfloat positionY, double radius)
	: PhysicsSprite("Content//Sprites//ball_1.png", "Ball", positionX, positionY),
	match(match),
	removeJoint(false),
	joint(NULL),
	owner(NULL),
	radius(radius)
{
}

Ball::~Ball(void)
{
	parent->enginePtr->world->DestroyBody(physicsObject.body);
}

void Ball::LoadContent()
{
	// Load the texture
	Sprite::LoadContent();

	// Create our circle object
	CirclePhysicsObject circleObject(radius);
	circleObject.CreateObject(parent->enginePtr->world, positionX, positionY, BALL, BOUNDARY | PLAYER | GOAL);
	circleObject.body->SetUserData(this);
	b2Fixture* fixture = circleObject.body->GetFixtureList();
	fixture->SetRestitution(0.7f);

	b2MassData massData;
	circleObject.body->GetMassData(&massData);
	massData.mass = .5;
	circleObject.body->SetMassData(&massData);
	circleObject.body->SetAngularDamping(1.0);
	circleObject.body->SetLinearDamping(0.5);

	// store our physics object
	physicsObject = circleObject;

	scaleX = scaleY = 0.5f;
}

void Ball::Update(float deltaTime)
{
	// Make sure the ball can't move if the game hasn't started
	if(!match->GameOn())
		physicsObject.body->SetLinearVelocity(b2Vec2(0,0));

	if(owner != NULL && joint == NULL)
	{
		// Get the direction the player is facing and normailse it
		b2Vec2 facing = owner->physicsObject.body->GetWorldVector(b2Vec2(1,0));
		facing.Normalize();

		// Get the position of the player
		b2Vec2 pos = owner->physicsObject.body->GetPosition();

		// Keep some space between the player and ball
		pos.x += facing.x * 0.5;
		pos.y += facing.y * 0.5;

		// Place the ball in front of the player
		physicsObject.body->SetTransform(pos, 0);

		physicsObject.body->SetLinearVelocity(b2Vec2(0,0));
		physicsObject.body->SetAngularVelocity(0);

		// Create a revolute joint to match the player's rotations
		b2RevoluteJointDef revjointDef;
		revjointDef.Initialize(owner->physicsObject.body, physicsObject.body, physicsObject.body->GetWorldCenter());
		revjointDef.lowerAngle = 0;
		revjointDef.upperAngle = 0;
		revjointDef.collideConnected = false;
		joint = (b2RevoluteJoint*)parent->enginePtr->world->CreateJoint(&revjointDef);

		// Tell the team it's in possession of the ball
		//owner->team->inPossession = true;
		owner->hasPossession = true;
	}

	if(removeJoint)
	{
		if(joint == NULL)
		{
			removeJoint = false;
			return;
		}

		// Remove the joint from the physics world
		parent->enginePtr->world->DestroyJoint(joint);
		joint = NULL;

		//owner->team->inPossession = false;
		owner->hasPossession = false;

		// Get the direction the player is facing
		b2Vec2 facing = owner->physicsObject.body->GetWorldVector(b2Vec2(1,0));
		facing.Normalize();

		if(owner->playerRole != Player::goal_keeper)
		{
			owner->SetTackled();

			owner = NULL;
		}

		physicsObject.body->ApplyLinearImpulse(facing, physicsObject.body->GetLocalCenter());

		removeJoint = false;
	}
}

void Ball::BeginContact(b2Contact* contact, GameComponent* collideWith)
{
	if(collideWith->name == "topGoal")
		match->Score(true);

	if(collideWith->name == "bottomGoal")
		match->Score(false);

	if(collideWith->name == "Player")
	{
		Player* collider = static_cast<Player*>(collideWith);

		if(match->GameOn())
		{
			if(owner == NULL)
				owner = collider;

			// Keeper gets ball whether or not a player already has control
			else if (collider->playerRole == Player::goal_keeper)
			{
				owner->SetTackled();

				if(joint != NULL)
				{
					// Remove the joint from the physics world
					removeJoint = true;
				}

				owner = collider;
			}
		}
		
		
	}
}

void Ball::Reset()
{
	if(joint != NULL)
	{
		// Remove the joint from the physics world
		parent->enginePtr->world->DestroyJoint(joint);
		joint = NULL;
	}

	if(owner != NULL)
	{
		//owner->team->inPossession = false;
		owner->hasPossession = false;
		owner = NULL;
	}

	physicsObject.body->SetTransform(b2Vec2(0,0), 0);
	physicsObject.body->SetLinearVelocity(b2Vec2(0,0));
	physicsObject.body->SetAngularVelocity(0);
}

void Ball::Pass(b2Vec2 dir, double power)
{
	if (owner == NULL)
		return;

	if(joint != NULL)
	{
		// Remove the joint from the physics world
		parent->enginePtr->world->DestroyJoint(joint);
		joint = NULL;
	}

	//owner->team->inPossession = false;
	owner->hasPossession = false;
	owner = NULL;

	dir *= power; // Kick Power

	physicsObject.body->ApplyLinearImpulse(dir, physicsObject.body->GetLocalCenter());
}

void Ball::Shoot(double power)
{
	if (owner == NULL)
		return;

	if(joint != NULL)
	{
		// Remove the joint from the physics world
		parent->enginePtr->world->DestroyJoint(joint);
		joint = NULL;
	}

	//owner->team->inPossession = false;
	owner->hasPossession = false;

	// Get the direction the player is facing
	b2Vec2 facing = owner->physicsObject.body->GetWorldVector(b2Vec2(1,0));
	facing.Normalize();
	owner = NULL;

	facing *= power; // Kick Power

	physicsObject.body->ApplyLinearImpulse(facing, physicsObject.body->GetLocalCenter());
}

void Ball::Free()
{
	if (owner == NULL)
		return;

	removeJoint = true;
}

void Ball::Push(b2Vec2 dir, double power)
{
	dir *= power; // Kick Power

	physicsObject.body->ApplyLinearImpulse(dir, physicsObject.body->GetLocalCenter());
}

//--------------------- FuturePosition -----------------------------------
//
//  given a time this method returns the ball position at that time in the
//  future
//------------------------------------------------------------------------
b2Vec2 Ball::FuturePosition(double time)const
{
	//using the equation s = ut + 1/2at^2, where s = distance, a = friction
	//u=start velocity

	//calculate the ut term, which is a vector
	b2Vec2 velocity = physicsObject.body->GetLinearVelocity();
	b2Vec2 ut(velocity.x * time, velocity.y * time);

	//calculate the 1/2at^2 term, which is scalar - -0.015 - FRICTION
	double half_a_t_squared = 0.5 * -0.015 * time * time;

	//turn the scalar quantity into a vector by multiplying the value with
	//the normalized velocity vector (because that gives the direction)
	velocity.Normalize();
	b2Vec2 ScalarToVector = half_a_t_squared * velocity;

	ScalarToVector *= METERS_TO_PIXELS; // change to pixel space

	//the predicted position is the balls position plus these two terms
	return GetPositionInPixels() + ut + ScalarToVector;
}

//---------------------- TimeToCoverDistance -----------------------------
//
//  Given a force and a distance to cover given by two vectors, this
//  method calculates how long it will take the ball to travel between
//  the two points
//------------------------------------------------------------------------
double Ball::TimeToCoverDistance(b2Vec2 A, b2Vec2 B, double force)const
{
	//this will be the velocity of the ball in the next time step *if*
	//the player was to make the pass. 
	double speed = force; // / m_dMass;

	//calculate the velocity at B using the equation
	//
	//  v^2 = u^2 + 2as
	//

	//first calculate s (the distance between the two positions)
	double DistanceToCover = DistanceSq(A, B);

	double term = speed*speed + 2.0*DistanceToCover * -0.015; //Prm.Friction;

	//if  (u^2 + 2as) is negative it means the ball cannot reach point B.
	if (term <= 0.0) 
		return -1.0;

	double v = sqrt(term);

	//it IS possible for the ball to reach B and we know its speed when it
	//gets there, so now it's easy to calculate the time using the equation
	//
	//    t = v-u
	//        ---
	//         a
	//
	return (v-speed)/ -0.015; //Prm.Friction;
}
