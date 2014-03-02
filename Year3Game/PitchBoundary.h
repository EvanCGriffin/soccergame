#pragma once

#include <Year3Engine\PhysicsObject.h>

// A boundary of the pitch
class PitchBoundary : public GameComponent
{
public:
	PitchBoundary(int positionX, int positionY, float halfWidth, float halfHeight);
	~PitchBoundary(void);

	void LoadContent();

	PhysicsObject physicsObject;

private:
	int positionX;
	int positionY;
	float halfWidth;
	float halfHeight;
};

