#include "BoxPhysicsObject.h"

BoxPhysicsObject::BoxPhysicsObject(float halfWidth, float halfHeight)
	: halfWidth(halfWidth),
	halfHeight(halfHeight)
{
}


BoxPhysicsObject::~BoxPhysicsObject(void)
{
}

void BoxPhysicsObject::CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
	uint16 categoryBits, uint16 maskBits, int16 groupIndex, bool isSensor)
{
	// Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(positionX * 0.04, positionY * 0.04);

	// Create the body
	body = world->CreateBody(&bodyDef);

	// Create the shape (box)
	b2PolygonShape shape;
	shape.SetAsBox(halfWidth, halfHeight);

	// Define material properties
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.isSensor = isSensor;

	//fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;

	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.filter.groupIndex = groupIndex;

	// Add the fixture to the body
	fixture = body->CreateFixture(&fixtureDef);
}
