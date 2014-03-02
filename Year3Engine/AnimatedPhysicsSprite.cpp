#include "AnimatedPhysicsSprite.h"
#include "Engine.h"

AnimatedPhysicsSprite::AnimatedPhysicsSprite(std::string filename, 
		std::string name, GLfloat x, GLfloat y, int framesPerSecond,
		int noOfAnimations,int framesPerAnimation, int frameWidth, 
		int frameHeight, int noOfRows)
		: AnimatedSprite(filename, name, x, y, framesPerSecond, noOfAnimations, framesPerAnimation, frameWidth, frameHeight, noOfRows)
{
}


AnimatedPhysicsSprite::~AnimatedPhysicsSprite(void)
{
	// Remove the body from the physics world
	if(physicsObject.body != NULL)
	{
		parent->enginePtr->world->DestroyBody(physicsObject.body);
		physicsObject.body = NULL;
	}
}

GLfloat AnimatedPhysicsSprite::getPositionX()
{
	return physicsObject.getPositionX();
}

GLfloat AnimatedPhysicsSprite::getPositionY()
{
	return physicsObject.getPositionY();
}

GLfloat AnimatedPhysicsSprite::getRotation()
{
	return physicsObject.getRotation();
}

b2Vec2 AnimatedPhysicsSprite::GetPositionInPixels() const
{
	return physicsObject.GetPositionInPixels();
}
