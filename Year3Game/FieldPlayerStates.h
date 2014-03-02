#pragma once

#include <Year3Engine\StateMachine.h>

class FieldPlayer;

class GlobalPlayerState : public State<FieldPlayer>
{
private:

	GlobalPlayerState(){}

public:

	//this is a singleton
	static GlobalPlayerState* Instance();

	void Enter(FieldPlayer* player){}

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&);
};

class ReturnToHomeRegion: public State<FieldPlayer>
{
private:

	ReturnToHomeRegion(){}

public:

	//this is a singleton
	static ReturnToHomeRegion* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){ return false;}
};

//------------------------------------------------------------------------
class Wait: public State<FieldPlayer>
{
private:

	Wait(){}

public:

	//this is a singleton
	static Wait* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class ChaseBall : public State<FieldPlayer>
{
private:

	ChaseBall(){}

public:

	//this is a singleton
	static ChaseBall* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class InPossession : public State<FieldPlayer>
{
private:

	InPossession(){}

public:

	//this is a singleton
	static InPossession* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class SupportAttacker: public State<FieldPlayer>
{
private:

	SupportAttacker(){}

public:

	//this is a singleton
	static SupportAttacker* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class ReceiveBall: public State<FieldPlayer>
{
private:

	ReceiveBall(){}

public:

	//this is a singleton
	static ReceiveBall* Instance();

	void Enter(FieldPlayer* player);

	void Update(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&) { return false; }
};