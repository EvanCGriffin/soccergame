#pragma once

#include <vector>
#include <Year3Engine\GameComponent.h>
#include <Year3Engine\StateMachine.h>

#include "Player.h"
#include "SupportSpotCalculator.h"

class SoccerMatch; // Forward Declaration
class FieldPlayer;

class Team : public GameComponent
{
	friend class Radar;
public:
	Team(SoccerMatch* match, bool human, bool home, std::string name, GLfloat r = 1, GLfloat g = 1, GLfloat b = 1, GLfloat a = 1);
	~Team(void);

	void LoadContent();
	void Update(float deltaTime);
	void Reset();
	// Finds the closest player to the ball and gives him control
	void CalcClosestPlayerToBall();
	// Finds a player to pass to given the players current direction, 
	// returns a player to pass to or NULL if none
	Player* FindPlayerToPassTo(Player* player);
	// Gets the direction between the player and the value returned 
	// from FindPlayerToPassTo and kicks the ball in theat direction
	void Pass(Player* player);

	int score;
	SoccerMatch* match;

	//bool inPossession; // is any of the players in possession of the ball
	GLfloat teamColor[4]; // The color of the team (R,G,B,A)

	bool const Home() const { return home; }
	bool const Human() const { return human; }

	//calling this changes the state of all field players to that of 
	//ReturnToHomeRegion. Mainly used when a goal keeper has
	//possession
	void ReturnAllFieldPlayersToHome();

	void SetOpponents (Team* opponents) { this->opponents = opponents; }
	Team* const Opponents() const { return opponents; }

	const std::vector<std::tr1::shared_ptr<Player>>& Members()const { return players; }

	//test if a pass from positions 'from' to 'target' kicked with force 
	//'PassingForce'can be intercepted by an opposing player
	bool isPassSafeFromOpponent(b2Vec2 from, b2Vec2 target, const Player* const receiver,
		const Player* const opp, double PassingForce)const;

	//tests a pass from position 'from' to position 'target' against each member
	//of the opposing team. Returns true if the pass can be made without
	//getting intercepted
	bool isPassSafeFromAllOpponents(b2Vec2 from, b2Vec2 target, 
		const Player* const receiver, double PassingForce)const;

	//returns true if player has a clean shot at the goal and sets ShotTarget
	//to a normalized vector pointing in the direction the shot should be
	//made. Else returns false and sets heading to a zero vector
	bool CanShoot(b2Vec2 BallPos, double power, b2Vec2& ShotTarget = b2Vec2())const;

	bool AllPlayersAtHome() const;

	void SetPlayerHomeRegion(int plyr, int region)const;

	void UpdateTargetsOfWaitingPlayers()const;

	Player* DetermineBestSupportingAttacker();

	bool isOpponentWithinRadius(b2Vec2 position, double radius);

	//The best pass is considered to be the pass that cannot be intercepted 
	//by an opponent and that is as far forward of the receiver as possible  
	//If a pass is found, the receiver's address is returned in the 
	//reference, 'receiver' and the position the pass will be made to is 
	//returned in the  reference 'PassTarget'
	bool FindPass(const Player*const passer,
		Player*& receiver, 
		b2Vec2& PassTarget,
		double power,
		double MinPassingDistance)const;

	//Three potential passes are calculated. One directly toward the receiver's
	//current position and two that are the tangents from the ball position
	//to the circle of radius 'range' from the receiver.
	//These passes are then tested to see if they can be intercepted by an
	//opponent and to make sure they terminate within the playing area. If
	//all the passes are invalidated the function returns false. Otherwise
	//the function returns the pass that takes the ball closest to the 
	//opponent's goal area.
	bool GetBestPassToReceiver(const Player* const passer, 
		const Player* const receiver, 
		b2Vec2& PassTarget, 
		const double power)const;


	inline b2Vec2 Perp(b2Vec2 vector) const
	{
		return b2Vec2(-vector.y, vector.x);
	}

	SupportSpotCalculator* SupportSpotCalc() { return supportSpotCalc; }

	Player* ControllingPlayer() const { return controllingPlayer; }
	void SetControllingPlayer(Player* player)
	{ 
		controllingPlayer = player; 
		if(player != NULL)
			Opponents()->LostControl();
	}

	bool InControl()const 
	{ 
		if(controllingPlayer) 
			return true; 
		else 
			return false;
	}

	void LostControl() { controllingPlayer = NULL; }

	Player* Closestplayer() { return closestPlayer; }
	void SetClosestPlayer(Player* player){ closestPlayer = player; }

	Player* SupportingPlayer() { return supportingPlayer; }
	void SetSupportingPlayer(Player* player){ supportingPlayer = player; }

	Player* Receiver() { return receiver; }
	void SetReceiver(Player* player){ receiver = player; }

	b2Vec2 GetSupportSpot()const { return supportSpotCalc->GetBestSupportingSpot(); }

	void DetermineBestSupportingPosition()const { supportSpotCalc->DetermineBestSupportingPosition(); }

	StateMachine<Team>* GetFSM() const { return stateMachine; }

	double ClosestDistToBallSq()const { return distSqToBallOfClosestPlayer; }

	//this tests to see if a pass is possible between the requester and
	//the controlling player. If it is possible a message is sent to the
	//controlling player to pass the ball asap.
	void RequestPass(FieldPlayer* requester)const;

private:
	bool home; // Used to select which way we play
	bool human; // Indicates whether this team is human controlled

	std::vector<std::tr1::shared_ptr<Player>> players;
	std::vector<std::tr1::shared_ptr<Player>>::iterator playerIter;

	Team* opponents;

	Player* closestPlayer; // The closest player to the ball
	Player* controllingPlayer;
	Player* supportingPlayer;
	Player* receiver;

	StateMachine<Team>* stateMachine;

	//players use this to determine strategic positions on the playing field
	SupportSpotCalculator*    supportSpotCalc;

	double distSqToBallOfClosestPlayer;
};

