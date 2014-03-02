#include "PhysicsSprite.h"
#include <Year3Engine\Utils.h>
#include "Engine.h"

PhysicsSprite::PhysicsSprite(std::string filename, std::string name, GLfloat x, GLfloat y)
	:Sprite(filename, name, x, y)
{
}

PhysicsSprite::~PhysicsSprite(void)
{
	// Remove the body from the physics world
	/*if(physicsObject.body != NULL)
	{
		parent->enginePtr->world->DestroyBody(physicsObject.body);
		physicsObject.body = NULL;
	}*/
}


GLfloat PhysicsSprite::getPositionX()
{
	return physicsObject.getPositionX();
}

GLfloat PhysicsSprite::getPositionY()
{
	return physicsObject.getPositionY();
}

GLfloat PhysicsSprite::getRotation()
{
	GLfloat angle = RADIANS_TO_DEGREES(physicsObject.getRotation());

	// Normailse the angle
	while (angle <= 0)
		angle += 360;
	
	while (angle > 360)
		angle -= 360;

	return angle;
	
}

b2Vec2 PhysicsSprite::GetPositionInPixels() const
{
	return physicsObject.GetPositionInPixels();
}