#include "CirclePhysicsObject.h"
#include <Box2D\Collision\Shapes\b2CircleShape.h>

CirclePhysicsObject::CirclePhysicsObject(float32 radius)
	:radius(radius)
{
}


CirclePhysicsObject::~CirclePhysicsObject(void)
{
}

void CirclePhysicsObject::CreateObject(b2World* world, GLfloat positionX, GLfloat positionY, 
	uint16 categoryBits, uint16 maskBits, int16 groupIndex)
{
	// Define the body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(positionX * 0.04, positionY * 0.04);

	// Create the body
	body = world->CreateBody(&bodyDef);

	// Create the shape (circle)
	b2CircleShape shape;
	shape.m_p.Set(0, 0);
	shape.m_radius = radius;	

	// Define material properties
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	fixtureDef.filter.groupIndex = groupIndex;

	// Add the fixture to the body
	fixture = body->CreateFixture(&fixtureDef);
}

