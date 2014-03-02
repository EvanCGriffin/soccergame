#pragma once

#include <Year3Engine\PhysicsObject.h>
#include <Year3Engine\Utils.h>

// The Goal object
class Goal : public GameComponent
{
public:
	Goal(int positionX, int positionY, float halfWidth, float halfHeight, std::string name);
	~Goal(void);

	void LoadContent();

	PhysicsObject physicsObject;

	double LeftPost() { return positionX - (halfWidth * METERS_TO_PIXELS); }
	double RightPost() { return positionX + (halfWidth * METERS_TO_PIXELS); }

	b2Vec2 GetPositionInPixels() const;

private:
	int positionX;
	int positionY;
	float halfWidth;
	float halfHeight;
};

