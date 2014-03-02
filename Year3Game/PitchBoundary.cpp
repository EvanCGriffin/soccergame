#include "PitchBoundary.h"

#include <Year3Engine\BoxPhysicsObject.h>
#include <Year3Engine\Engine.h>

#include "SoccerMatch.h"

PitchBoundary::PitchBoundary(int positionX, int positionY, float halfWidth, float halfHeight)	
	: GameComponent("Boundary"),
	positionX(positionX), 
	positionY(positionY),
	halfWidth(halfWidth),
	halfHeight(halfHeight)
{
}

PitchBoundary::~PitchBoundary(void)
{
	// Remove the body from the physics world
	parent->enginePtr->world->DestroyBody(physicsObject.body);
}

void PitchBoundary::LoadContent()
{
	// Create our circle object
	// Create our box object
	BoxPhysicsObject boxObject(halfWidth, halfHeight);
	boxObject.CreateObject(parent->enginePtr->world, positionX, positionY, BOUNDARY, PLAYER | BALL | TACKLED_PLAYER);
	boxObject.body->SetUserData(this);

	// store our physics object
	physicsObject = boxObject;
}
