#pragma once

#include "AnimatedSprite.h"
#include "PhysicsObject.h"

// Draws an animated sprite at its physics object's position & rotation
class AnimatedPhysicsSprite : public AnimatedSprite
{
public:
	AnimatedPhysicsSprite(std::string filename, std::string name, GLfloat x = 0, GLfloat y = 0, int framesPerSecond = 5,
		int noOfAnimations = 1, int framesPerAnimation = 1, int frameWidth = 0, 
		int frameHeight = 0, int noOfRows = 1);

	~AnimatedPhysicsSprite(void);


	GLfloat getPositionX(void);
	GLfloat getPositionY(void);
	GLfloat getRotation(void);
	b2Vec2 GetPositionInPixels() const;

	// The object in the physics world
	PhysicsObject physicsObject;
};

