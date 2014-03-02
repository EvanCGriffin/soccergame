#pragma once

#include <Box2D/Box2D.h>
#include <SDL\SDL_opengl.h>

#include "GameScreen.h"

// Holds a Physics body 
class PhysicsObject
{
public:
	PhysicsObject(void);
	virtual ~PhysicsObject(void);

	// Create the physics object at x, y, and set collision properties
	virtual void CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
		uint16 categoryBits = 1, uint16 maskBits = 1, int16 groupIndex = 0);

	virtual GLfloat getPositionX();
	virtual GLfloat getPositionY();
	virtual GLfloat getRotation();
	virtual b2Vec2 GetPositionInPixels() const;

	b2Body* body;
	b2Fixture* fixture;
};

