#pragma once
#include <Year3Engine/AnimatedPhysicsSprite.h>
#include <Year3Engine/PhysicsSprite.h>
#include <Year3Engine\Utils.h>
#include "PitchRegion.h"
#include "Smoother.h"


const int PLAYER_SPEED = 4;

class Team; // Forward Declaration
class SteeringBehaviors;

// A Player object
class Player : public AnimatedPhysicsSprite
{
	friend class Ball;
public:

	enum player_role{goal_keeper, attacker, defender};

	Player(Team* team, int homeRegion, player_role role, bool dummyPlayer = false, GLfloat x = 0, GLfloat y = 0);
	~Player(void);

	// Load the mask, player indicator sprites and create the physics object
	void LoadContent();
	// Handle Player inputs
	void HandleInput(Input input);
	// Update tackling timer and animation
	void Update(float deltaTime);
	// Draw the player indicator, the mask and the player sprite
	void Draw();

	// No rotation - returns 0
	GLfloat getRotation(void);

	double MaxForce() const { return maxForce; }
	void SetMaxForce(double mf) { maxForce = mf; }


	double PassPower() const { return passPower; }
	double ShotPower() const { return shotPower; }

	double MaxSpeed() const { return maxSpeed; }
	void SetMaxSpeed(double ms) { maxSpeed = ms; }

	// Check for tackles
	void BeginContact(b2Contact* contact, GameComponent* collideWith);
	// Reset the player to the starting position
	void Reset();
	// Set the player as tackled
	void SetTackled();

	bool player2; // input mode
	bool dummyPlayer; // static player
	bool isControlled; // currently controlled
	bool hasPossession; // have the ball
	bool tackling; // is tackling
	bool tackled; // has been tackled
	float radius; // radius of the physics object

	float tackledTimer; // Time remaining being tackled
	Team* team; // The team

	SteeringBehaviors* const Steering() const { return steering; }

	player_role Role()const { return playerRole; }

	void SetDefaultHomeRegion(){ homeRegion = defaultRegion; }

	void SetHomeRegion(int NewRegion){ homeRegion = NewRegion; }

	const PitchRegion* const HomeRegion() const;

	bool AtTarget();

	bool InHomeRegion() const;

	//returns true if the player is the closest player in his team to
	//the ball
	bool isClosestTeamMemberToBall()const;

	//returns true if the player is the closest player on the pitch to the ball
	bool isClosestPlayerOnPitchToBall()const;

	bool InHotRegion() const;

	void FindSupport();

	void ClampToDirection(b2Vec2 &vector);

	//returns true if the ball can be grabbed by the goalkeeper
	bool BallWithinKeeperRange()const;

	double DistSqToBall()const { return distSqToBall; }
	void SetDistSqToBall(double val) { distSqToBall = val; }

	player_role PlayerRole() { return playerRole; }

	bool isControllingPlayer()const;
	bool isAheadOfAttacker()const;

	bool isThreatened()const;

	bool PositionInFrontOfPlayer(b2Vec2 position)const;

	bool CloseEnoughToTackle()const;

protected:

	//this player's role in the team
	player_role playerRole;

	//the region that this player is assigned to.
	int  homeRegion;

	//the region this player moves to before kickoff
	int defaultRegion;

	Smoother<b2Vec2>* smoother;
	b2Vec2 startingPos; // The starting position
	b2Vec2 previousPosition;
	GLuint maskTexture; // The mask texture (color texture)
	PhysicsSprite controlledSprite; // The player indicator sprite
	b2Vec2 velocity;
	SteeringBehaviors* steering;
	double maxForce;
	double passPower;
	double shotPower;
	double maxSpeed;
	double distSqToBall;
};

