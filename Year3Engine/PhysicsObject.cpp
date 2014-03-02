#include "PhysicsObject.h"
#include "Utils.h"

PhysicsObject::PhysicsObject()
{
	body = NULL;
}

PhysicsObject::~PhysicsObject(void)
{
}

void PhysicsObject::CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
	uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
}

GLfloat PhysicsObject::getPositionX()
{
	if (body == NULL)
		return 0;

	float32 x = body->GetPosition().x;

	x *= METERS_TO_PIXELS;

	return x;
}

GLfloat PhysicsObject::getPositionY()
{
	if (body == NULL)
		return 0;

	float32 y = body->GetPosition().y;

	y *= METERS_TO_PIXELS;

	return y;
}

GLfloat PhysicsObject::getRotation()
{
	if (body == NULL)
		return 0;

	return body->GetAngle();
}

b2Vec2 PhysicsObject::GetPositionInPixels() const
{
	if (body == NULL)
		return b2Vec2(0,0);

	b2Vec2 pos = body->GetPosition();
	pos *= METERS_TO_PIXELS;
	return pos;
}