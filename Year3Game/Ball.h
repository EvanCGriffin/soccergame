#pragma once

#include <Year3Engine\PhysicsSprite.h>
#include "Player.h"

class SoccerMatch; // Forward Declaration

// The ball
class Ball : public PhysicsSprite
{
public:
	Ball(SoccerMatch* match, GLfloat positionX = 0, GLfloat positionY = 0, double radius = 0.2);
	~Ball(void);

	// Create the physics object and load the sprite
	void LoadContent();
	void Update(float deltaTime);

	void BeginContact(b2Contact* contact, GameComponent* collideWith);

	void Reset();

	// Applies a force to the ball in the direction
	void Pass(b2Vec2 dir, double power);
	// Applies a force to the ball in the direction of the owner
	void Shoot(double power);
	// Detach the ball from its owner
	void Free();

	// Used for starting 
	void Push(b2Vec2 dir, double power);

	double Radius() { return radius * METERS_TO_PIXELS; }

	//this method calculates where the ball will in 'time' seconds
	b2Vec2 FuturePosition(double time)const;

	//given a kicking force and a distance to traverse defined by start
	//and finish points, this method calculates how long it will take the
	//ball to cover the distance.
	double TimeToCoverDistance(b2Vec2 from, b2Vec2 to, double force)const;


	Player* Owner() { return owner; }

private:
	bool removeJoint; // tells whether to detach the ball from the player
	Player* owner; // The player the ball is currently attached to
	b2RevoluteJoint* joint; // The joint attaching the ball to the player
	SoccerMatch* match; // The current match
	double radius;
};

