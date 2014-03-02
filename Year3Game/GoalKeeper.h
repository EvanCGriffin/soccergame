#pragma once

#include <Year3Engine/StateMachine.h>

#include "Player.h"


class GoalKeeper : public Player
{
public:
	GoalKeeper(Team* team, int homeRegion, State<GoalKeeper>* startState, bool dummyPlayer = false, GLfloat x = 0, GLfloat y = 0);
	~GoalKeeper(void);

	void Update(float deltaTime);

	bool HandleMessage(const Telegram& msg);

	//void BeginContact(b2Contact* contact, GameComponent* collideWith); 

	StateMachine<GoalKeeper>* GetFSM()const { return stateMachine; }

	//returns true if the ball comes close enough for the keeper to 
   //consider intercepting
   bool        BallWithinRangeForIntercept()const;

   //returns true if the keeper has ventured too far away from the goalmouth
   bool        TooFarFromGoalMouth()const;

   //this method is called by the Intercept state to determine the spot
   //along the goalmouth which will act as one of the interpose targets
   //(the other is the ball).
   //the specific point at the goal line that the keeper is trying to cover
   //is flexible and can move depending on where the ball is on the field.
   //To achieve this we just scale the ball's y value by the ratio of the
   //goal width to playingfield width
   b2Vec2    GetRearInterposeTarget()const;

private:
	StateMachine<GoalKeeper>* stateMachine;
};

