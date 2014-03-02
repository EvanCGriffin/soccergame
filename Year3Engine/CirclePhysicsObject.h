#pragma once

#include "PhysicsObject.h"

// A circle physics body
class CirclePhysicsObject : public PhysicsObject
{
public:
	CirclePhysicsObject(float32 radius);
	~CirclePhysicsObject(void);

	// Creates a circle body at x, y, with the specfied collision params
	void CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
		uint16 categoryBits = 1, uint16 maskBits = 1, int16 groupIndex = 0);

private:
	float32 radius; // The radius of the circle
};

