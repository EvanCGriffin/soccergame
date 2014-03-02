#pragma once

#include "Player.h"
#include <Year3Engine\StateMachine.h>

class FieldPlayer : public Player
{
public:
	FieldPlayer(Team* team, int homeRegion, State<FieldPlayer>* startState, player_role role, bool dummyPlayer = false, GLfloat x = 0, GLfloat y = 0);
	~FieldPlayer(void);

	void Update(float deltaTime);

	bool HandleMessage(const Telegram& msg);

	StateMachine<FieldPlayer>* GetFSM()const { return stateMachine; }

private:
	StateMachine<FieldPlayer>* stateMachine;
};

