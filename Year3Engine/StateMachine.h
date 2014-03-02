#pragma once

#include <assert.h>

struct Telegram;

template <class T>
class State
{
public:

	virtual ~State() {}

	//this will execute when the state is entered
	virtual void Enter(T*)=0;

	//this is the states normal update function
	virtual void Update(T*)=0;

	//this will execute when the state is exited. 
	virtual void Exit(T*)=0;

	//this executes if the agent receives a message from the 
	//message dispatcher
	virtual bool OnMessage(T*, const Telegram&)=0;
};

template <class T>
class StateMachine
{
public:
	StateMachine(T* owner)
		:owner(owner),
		currentState(0),
		previousState(0),
		globalState(0)
	{
	}

	virtual ~StateMachine() {}

	//use these methods to initialize the FSM
	void SetCurrentState(State<T>* state) { currentState = state; }
	void SetGlobalState(State<T>* state) { globalState = state; }
	void SetPreviousState(State<T>* state) { previousState = state; }

	//call this to update the FSM
	void Update() const
	{
		//if a global state exists, update it
		if(globalState)
			globalState->Update(owner);

		//same for the current state
		if(currentState)
			currentState->Update(owner);
	}

	bool  HandleMessage(const Telegram& msg)const
	{
		//first see if the current state is valid and that it can handle
		//the message
		if (currentState && currentState->OnMessage(owner, msg))
		{
			return true;
		}

		//if not, and if a global state has been implemented, send 
		//the message to the global state
		if (globalState && globalState->OnMessage(owner, msg))
		{
			return true;
		}

		return false;
	}

	//change to a new state
	void  ChangeState(State<T>* newState)
	{
		assert(newState && "<StateMachine::ChangeState>:trying to assign null state to current");

		//keep a record of the previous state
		previousState = currentState;

		//call the exit method of the existing state
		currentState->Exit(owner);

		//change state to the new state
		currentState = newState;

		//call the entry method of the new state
		currentState->Enter(owner);
	}

	//change state back to the previous state
	void RevertToPreviousState()
	{
		ChangeState(previousState);
	}

	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
	bool isInState(const State<T>& state) const
	{
		if (typeid(*currentState) == typeid(state))
			return true;

		return false;
	}

	State<T>*  CurrentState()  const { return currentState; }
	State<T>*  GlobalState()   const { return globalState; }
	State<T>*  PreviousState() const { return previousState; }

private:
	T* owner; // The owner of this state machine
	State<T>* currentState; // The current state 
	State<T>* previousState; // Keep a record of the previous state
	State<T>* globalState; // 
};

