#include "Goal.h"

#include <Year3Engine\BoxPhysicsObject.h>
#include <Year3Engine\Engine.h>

#include "SoccerMatch.h"

Goal::Goal(int positionX, int positionY, float halfWidth, float halfHeight, std::string name)	
	: GameComponent(name),
	positionX(positionX), 
	positionY(positionY),
	halfWidth(halfWidth),
	halfHeight(halfHeight)
{
}


Goal::~Goal(void)
{
	// Remove the body from the physics world
	parent->enginePtr->world->DestroyBody(physicsObject.body);
}

void Goal::LoadContent()
{
	// Create our circle object
	// Create our box object
	BoxPhysicsObject boxObject(halfWidth, halfHeight);
	boxObject.CreateObject(parent->enginePtr->world, positionX, positionY, GOAL, BALL, 0, true);
	boxObject.body->SetUserData(this);

	// store our physics object
	physicsObject = boxObject;
}

b2Vec2 Goal::GetPositionInPixels() const
{
	return b2Vec2(positionX, positionY);
}
