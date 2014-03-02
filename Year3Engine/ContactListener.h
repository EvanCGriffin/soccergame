#pragma once

#include <Box2D\Box2D.h>

// Records all colisions, calls BeginContact and 
// EndContact on any attached GameComponents
class ContactListener : public b2ContactListener
{
public:
	// Called when a new collision occurs
	void BeginContact(b2Contact* contact);
	// Called when two objects are no longer colliding
	void EndContact(b2Contact* contact);
};

