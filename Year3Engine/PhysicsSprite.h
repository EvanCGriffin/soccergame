#pragma once

#include "Sprite.h"
#include "PhysicsObject.h"

// Draws a sprite at it's physics object's position & rotation
class PhysicsSprite : public Sprite
{
public:
	PhysicsSprite(std::string filename, std::string name = "", GLfloat x = 0, GLfloat y = 0);
	virtual ~PhysicsSprite(void);

	// returns the physics object position and rotation
	GLfloat getPositionX(void);
	GLfloat getPositionY(void);
	GLfloat getRotation(void);
	b2Vec2 GetPositionInPixels() const;

	// The object in the physics world
	PhysicsObject physicsObject;
};

