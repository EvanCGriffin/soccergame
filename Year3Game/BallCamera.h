#pragma once
#include <Year3Engine\Camera.h>
#include <Year3Engine\GameScreen.h>
#include "Ball.h"

// A camera that follows the ball
class BallCamera : public Camera
{
public:
	BallCamera(GameScreen* parent, Ball* ball);
	~BallCamera(void);

	// Set the camera's x and y to the ball's x and y
	// TODO lerp between positions for smooth movement
	void Update(float deltaTime);

private:
	GameScreen* parent; // The screen this component is added to
	Ball* ball; // the ball to follow
};

