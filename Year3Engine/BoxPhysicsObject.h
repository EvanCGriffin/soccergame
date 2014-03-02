#pragma once

#include "PhysicsObject.h"

// A square physics body
class BoxPhysicsObject : public PhysicsObject 
{
public:
	BoxPhysicsObject(float halfWidth, float halfHeight);
	~BoxPhysicsObject(void);

	// Creates a square body at x, y, with the specfied collision params
	void CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
		uint16 categoryBits = 1, uint16 maskBits = 1, int16 groupIndex = 0, bool isSensor = false);

private:
	float halfWidth; // The width of the box from its center
	float halfHeight; // The height of the box from its center
};

