#include "BallCamera.h"
#include <Year3Engine\Engine.h>

BallCamera::BallCamera(GameScreen* parent, Ball* ball)
	:ball(ball),
	parent(parent)
{
}


BallCamera::~BallCamera(void)
{
}

// Set our camera position to the position of the ball
void BallCamera::Update(float deltaTime)
{
	if(ball != NULL)
	{
		positionX = (parent->enginePtr->ScreenWidth / 2) - ball->getPositionX();
		positionY = (parent->enginePtr->ScreenHeight / 2) - ball->getPositionY();
	}
}
